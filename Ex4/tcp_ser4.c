/**********************************
tcp_ser.c: the source file of the server in tcp transmission 
***********************************/


#include "headsock.h"

#define BACKLOG 10

void str_ser(int sockfd, float error_probability, int packet_length);  // transmitting and receiving function
void tv_sub(struct  timeval *out, struct timeval *in); //calcu the time interval between out and in

int main(int argc, char **argv)
{
	int sockfd, con_fd, ret;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
    float error_probability = atof(argv[1]);
    int packet_length = atof(argv[2]);
    
    printf("The error probability is: %f \n", error_probability);


//	char *buf;
	pid_t pid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYTCP_PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("172.0.0.1");
	bzero(&(my_addr.sin_zero), 8);
	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	ret = listen(sockfd, BACKLOG); //listen
	if (ret <0) {
		printf("error in listening");
		exit(1);
	}

	while (1)
	{
		printf("waiting for data\n");
		sin_size = sizeof (struct sockaddr_in);
		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);            //accept the packet
		if (con_fd <0)
		{
			printf("error in accept\n");
			exit(1);
		}

		if ((pid = fork())==0)  // creat acception process
		{
			close(sockfd);
			str_ser(con_fd, error_probability, packet_length);  //receive packet and response
			close(con_fd);
			exit(0);
		}
		else close(con_fd); //parent process
	}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd, float error_probability, int packet_length)
{
	char buf[BUFSIZE];
	FILE *fp;
	char recvs[packet_length];
	struct ack_so ack;
	int end, n = 0;
    int packetNo = 0;
	long lseek=0;
	struct timeval sendt, recvt;
	float time_inv = 0.0;
	float ti, rt;
    
	end = 0;
	
	printf("receiving data!\n");

	gettimeofday(&sendt, NULL);
	while(!end)
	{      
        packetNo ++;
        //receive the packet
		if ((n= recv(sockfd, &recvs, packet_length, 0))==-1) {
			printf("error when receiving\n");
			exit(1);
		}
        
        // Sending the acknowledgment
        
        // Simulating an error
        int pick = rand() % 1000;
        int threshold = (int)(1000*error_probability);

        // Packet corrupted
        if (pick < threshold){
            ack.num = 0;

            printf("Packet corrupted, NACK sent...");
        // Packet good!
        } else {
            ack.num = 1;

            memcpy((buf+lseek), recvs, n);
			lseek += n;

			// check if it is the end of the file
			if (recvs[n-1] == '\0')	{
				end = 1;
				n --;
			}

			printf("Packet good, ACK sent...");
        }
        
        ack.len = 0;
        //send the ack
        if ((n = send(sockfd, &ack, 2, 0))==-1) {
            printf("send error!"); 
            exit(1);
        } else {
            printf("Ack %d sent \n", packetNo);
        }
	}
	gettimeofday(&recvt, NULL);

	if ((fp = fopen ("myTCPreceive.txt","wt")) == NULL)
	{
		printf("File doesn't exit\n");
		exit(0);
	}
	fwrite (buf , 1 , lseek , fp);	//write data into file
	fclose(fp);
	printf("A file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);

	// Checking if the two files match 
	FILE *myFile, *myTCPreceive;
    int myFileStream, myTCPreceiveStream;
    int bytesCorrect = 0;

	myFile = fopen ("myfile.txt","r+t");
	myFileStream = getc(myFile);
	myTCPreceive = fopen ("myTCPreceive.txt","r+t");
	myTCPreceiveStream = getc(myTCPreceive);

	while((myFileStream != EOF) && (myTCPreceiveStream != EOF) && (myFileStream == myTCPreceiveStream)) {
		bytesCorrect++;
		myFileStream = getc(myFile);
		myTCPreceiveStream = getc(myTCPreceive);
	}

	// For some reason the transmission removes the last byte
	if (bytesCorrect == (int)lseek-1) {
		printf("The received file MATCHES the sent one! \n");
	} else {
		printf("The received file DOES NOT match the sent one! \n");
	}

	// Calculating the transmission time
	tv_sub(&recvt, &sendt); // get the whole trans time
	time_inv += (recvt.tv_sec)*1000.0 + (recvt.tv_usec)/1000.0;
	ti = time_inv;
	rt = (lseek/(float)ti);

	printf("Time(ms) : %.3f, Data sent(byte): %d\nData rate: %f (Kbytes/s)\n", ti, (int)lseek, rt);

	FILE *results;
	char result[] = "\n ";
	std::ostringstream myString;

	myString<<ti << " " <<result;

	results = fopen("results.txt", "a");
	fputs(result, results);
    fclose(results);

}

void tv_sub(struct  timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) <0)
	{
		--out ->tv_sec;
		out ->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}