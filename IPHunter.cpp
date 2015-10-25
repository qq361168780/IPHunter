#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <math.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")


int main()
{
	//计数：连接总数
	unsigned int count=0;

	 //初始化WSA
     WORD sockVersion = MAKEWORD(2,2);
     WSADATA wsaData;
     if(WSAStartup(sockVersion, &wsaData)!=0)
     {
         return 0;
     }
 
     //创建套接字
     SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if(slisten == INVALID_SOCKET)
     {
         printf("socket error !");
         return 0;
     }
 
     //绑定IP和端口
	 u_short portNum=80;
     sockaddr_in sin;
     sin.sin_family = AF_INET;
     sin.sin_port = htons(portNum);
     sin.sin_addr.S_un.S_addr = INADDR_ANY; 
     if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
     {
TAG1:   printf("bind error ! port %d is used !\n",portNum);
		//reset port
		printf("please input new port:\n");
		scanf("%us",&portNum);
		sin.sin_port = htons(portNum);
		while(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			goto TAG1;
		}
     }
 
     //开始监听
     if(listen(slisten, 5) == SOCKET_ERROR)
     {
         printf("listen error !");
		 system("pause");
         return 0;
     }
 
     //循环接收数据
     SOCKET sClient;
     sockaddr_in remoteAddr;
     int nAddrlen = sizeof(remoteAddr);
     char revData[255]; 
     while (true)
     {

		 printf("\n%u ___________________________________________________________\n",count);
         printf("waiting Linking...\n");
         sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
         if(sClient == INVALID_SOCKET)
         {
             printf("accept error !");
             continue;
         }
         printf("accept a link：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		 if (count<UINT_MAX)
		 {
			 count++;
		 }

		 struct tm *local; 
		 time_t t; 
		 t=time(NULL); 
		 local=localtime(&t); 
		 printf("____Local hour is: %d/%d/%d-%d:%d:%d_____\n",local->tm_year+1900,(local->tm_mon+1),local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec); 

         //接收数据
         int ret = recv(sClient, revData, 255, 0);        
         if(ret > 0)
         {
             revData[ret] = 0x00;
             printf(revData);
         }
		 //closesocket(sClient);
 
         //发送数据
		 char * sendData = "<html><head> <title>连接失败</title><head/><html/>";
		 send(sClient, sendData, strlen(sendData), 0);
		 closesocket(sClient);
     }
     
     closesocket(slisten);
     WSACleanup();
     return 0;

	
}