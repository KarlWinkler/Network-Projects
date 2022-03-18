// router.c


#include <string.h>

#include "helper.c"
#include "fileReader.c"
#include "packet.h"

// #define buffer_size 100
// #define bandwidth (10000000/8)

int main(int argc, char* argv[]){
    long len = 0;
    int err = 0;
    double current_time = 0;
    char **trace;
    char **second_trace;
    struct packet *packets;
    int in_buffer = 0;
    int work_left = 0;
    double total_delay = 0;
    long lost = 0;
    long sum_lost = 0;
    long sum_in = 0;
    long sum_out = 0;

    int buffer_size = 100;
    long bandwidth = 5000000;

    int flags = 0;

    if(argc > 1){
        if(strcmp(argv[1], "-c") == 0){ // -c ombine
            flags+=2;
        
            long len_a;
            long len_b;
            
            trace = file_reader(argv[2], &len_a, &err);
            second_trace = file_reader(argv[3], &len_b, &err);
            len = len_a+len_b;

            struct packet *trace_packets = toPackets(trace, len_a, &sum_in);
            struct packet *second_trace_packets = toPackets(second_trace, len_b, &sum_in);
            packets = merge_sort(trace_packets, len_a, second_trace_packets, len_b);
            // packets = trace_packets;

            free(second_trace);

        }
    }

    if(argc > 2 + flags){
        buffer_size = atoi(argv[2 + flags]);
    }

    if(argc > 3 + flags){
        bandwidth = atoi(argv[3 + flags]);
    }

    bandwidth *= 0.125;

    if(argc > 1 && flags == 0){
        trace = file_reader(argv[1], &len, &err);
        packets = toPackets(trace, len, &sum_in);

        if(err == 1){
            exit(err);
        }
    }
    else if(argc <= 1){
        printf("No file given as argument\n");
        exit(1);
    }

    struct packet *buffer = malloc(buffer_size * sizeof(struct packet));


    // len = 110;



    // increment time by next packet then process it
    for(int i = 0; i < len; i++){
        

        // printf("%f : %d : buf %d\n", packets[i].arrival_time, packets[i].size, in_buffer);
        double time_increment = 0; // time passed processing packet

        double time_block = packets[i].arrival_time - current_time; // timeframe this itteration is working with


        //handel buffer jobs after initial process

        while(time_block > 0){
            // printf("%f : %d\n", time_block, work_left);
            // printf("%d\n", packets[i].size);
            //if there is work left, finish it
            if(work_left > 0){
                //decrement work left then remove time spent from the 
                int temp_work_left = work_left;
                // printf("%d, %f %d\n", work_left, time_block, i);
                if((work_left -= time_block * bandwidth) <= 0){
                    time_increment = (temp_work_left / (double)bandwidth);
                    // printf("%f\n", time_increment);
                    sum_out += temp_work_left; 
                    work_left = 0;
                // printf("%f : %f\n", current_time, packets[i].arrival_time);

                }
                else{
                    time_increment = time_block;
                }
                current_time += time_increment;
                time_block -= time_increment;
            }
            else if(in_buffer > 0){
                work_left = buffer[0].size;
                total_delay += current_time - buffer[0].arrival_time; // calc queuing delay

                // consume head of buffer
                for(int i = 0; i < in_buffer - 2; i++){ // move everything in buffer forwards
                    buffer[i] = buffer[i+1];
                }
                in_buffer--;
            }
            else{
                break;
            }
        }
        
        //handel new packet arriving
        if(in_buffer >= buffer_size){ // full ap
           lost++; 
            sum_lost += packets[i].size;
        }
        else if(in_buffer == 0 && work_left <= 0){ // empty AP 
            work_left = packets[i].size;
            buffer[in_buffer] = packets[i];
            in_buffer++;
        }
        else{ // ap in use
            buffer[in_buffer] = packets[i];
            in_buffer++;
        }

        current_time = packets[i].arrival_time; // match up time
    }

    printf("=====================================\n");
    printf("Buffer Size: %d Packets\n", buffer_size);
    printf("Link Speed: %ldBps\n", bandwidth * 8);

    //// debug 
    // printf("Total Time: %f\n", current_time);
    // printf("Packets in: %d %d\n", len , sum_in);
    // printf("Packets out:%d %d\n", len , sum_out);
    // printf("Packets lost: %d | %f\n", lost);
    // printf("total delay: %f\n", total_delay);

    printf("=====================================\n");
    printf("queuing delay: %f\n", total_delay / len);
    printf("lost packets: %f\n", lost / (double)len);

    free(packets);
    free(buffer);
    destroy_array(trace);
}





// increment time by next packet then process it
    // for(int i = 0; i < len; i++){
        
    //     double time_increment = 0; // time passed processing packet

    //     double time_block = packets[i].arrival_time - current_time; // timeframe this itteration is working with

    //     // process current job
    //     if(work_left > 0){
    //         //decrement work left then add package delay to total delay
    //         int temp_work_left = work_left;
    //         work_left -= time_block * bandwidth;
    //         if(work_left <= 0){ 
    //             time_increment = (temp_work_left / (double)bandwidth); // calculate time passed
    //             // printf("%f : %f\n", current_time, buffer[0].arrival_time);
    //             busy = 0;
    //         } 
    //         // adjust time keeping
    //         current_time += time_increment;
    //         time_block -= time_increment;
    //     }


    //     //handel buffer jobs after initial process
    //     while(in_buffer > 0 && time_block > 0){

    //         if(work_left <= 0){
    //             busy = 1;
    //             work_left = buffer[0].size;
    //             total_delay += current_time - buffer[0].arrival_time; // calc queuing delay
    //             for(int i = 0; i < in_buffer - 1; i++){ // move everything in buffer forwards
    //                 buffer[i] = buffer[i+1];
    //             }
    //             in_buffer--;
    //             //decrement work left then remove time spent from the 
    //             int temp_work_left = work_left;
    //             if((work_left -= time_block * bandwidth) <= 0){
    //                 time_increment = (temp_work_left / (double)bandwidth);
    //                 // total_delay += work_left_temp / (time_increment * bandwidth); // calculate remaining wait time
    //                 busy = 0;
    //             }
    //             current_time += time_increment;
    //             time_block -= time_increment;

    //             // printf("%d\n", in_buffer);

    //         }
    //         else{
    //             break;
    //         }
    //     }
        
    //     //handel new packet arriving
    //     if(busy == 0 && in_buffer == 0){
    //         busy = 1;
    //         work_left = packets[i].size;
    //     }
    //     else if(busy == 0 && in_buffer > 0){
    //         busy = 1;
    //         work_left = buffer[0].size;
    //         total_delay += current_time - buffer[0].arrival_time;
    //         for(int i = 0; i < in_buffer - 1; i++){
    //             buffer[i] = buffer[i+1];
    //         }

    //         in_buffer--;
    //     }
    //     else if(busy == 1 && in_buffer < buffer_size){
    //         buffer[in_buffer] = packets[i];
    //         in_buffer++;
    //     }
    //     else{
    //         lost++;
    //     }

    //     current_time += packets[i].arrival_time - current_time;

    //     // printf("%f\n", total_delay);
    //     // if(current_time == 0){
            
    //     //     break;
    //     // }
    // }