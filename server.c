#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_ROWS 1000
#define MAX_COLS 10
#define MAX_DATE_LEN 11

typedef struct {
  char date[MAX_DATE_LEN];
  double closing_price;
} StockData;


int checkdate(const char* date) {
    int year, month, day;
    
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) == 3) {
        if (year >= 0 && month >= 1 && month <= 12 && day >= 1 && day <= 31) {
            return 1; 
        }
    }

    return 0;
}
void readCSV(const char *filename, StockData *stockData, int *numRows) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file");
  } else {
    char line[256];
    fgets(line, sizeof(line), fp);
    *numRows = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
      sscanf(line, "%10[^,],%*lf,%*lf,%*lf,%lf", stockData[*numRows].date,
             &stockData[*numRows].closing_price);
      (*numRows)++;
    }

    fclose(fp);
  }
}

/*char* listfunc(){
  return "TSLA | MSFT";

}*/

/*char* pricesfunc(companyname,targetDate){
  int found=0;
    if(strcmp(companyname, "TSLA") == 0){
      for (int i = 0; i < sizeof(tslaData); i++) {
      if (strcmp(tslaData[i].date, targetDate) == 0) {
        // Date found, format the output
        sprintf(buffer, "Date: %s, Closing Price: %.2lf\n", tslaData[i].date,
                tslaData[i].closing_price);
                found=1;
        break;
      }
    }

    }
    else if(strcmp(companyname,"MSFT")==0){
      for (int i = 0; i < sizeof(msftData); i++) {
      if (strcmp(msftData[i].date, targetDate) == 0) {
        // Date found, format the output
        sprintf(buffer, "Date: %s, Closing Price: %.2lf\n", msftData[i].date,
                msftData[i].closing_price);
        break;
      }
    }

    }
    if(found==1){
      return buffer;
    }
    else{
      buffer="Date not found";
      return buffer;
    }


}*/

int main(int argc, char *argv[]) {
  StockData msftData[MAX_ROWS];
  StockData tslaData[MAX_ROWS];
  int msftRows, tslaRows;

  // Read data from MSFT.csv
  readCSV("MSFT.csv", msftData, &msftRows);
  readCSV("TSLA.csv", tslaData, &tslaRows);
  /*printf("MSFT Data:\n");
  for (int i = 0; i < msftRows; i++) {
    printf("Date: %s, Closing Price: %.2lf\n", msftData[i].date,
           msftData[i].closing_price);
  }*/

  int sockfd, portno, newsockfd, n;
  char buffer[255];
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t cli_len;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Error opening the socket");
    exit(0);
  } else {
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[3]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("Bind failed!");
      exit(0);
    }

    listen(sockfd, 5);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &(cli_len));

    if (newsockfd < 0) {
      printf("Accepting Error");
      exit(0);
    }
    printf("server started\n");
    while (1) {
      bzero(buffer, 255);
      n = read(newsockfd, buffer, 255);
      if (n < 0) {
        printf("Error reading");
        exit(0);
      }
      int strLength = buffer[0];
      printf("%.*s", strLength, buffer + 1);
      char command[128] = "";
      char args1[128] = "";
      char args2[128] = "";
      char args3[128] = "";
      int command_count =
          sscanf(buffer + 1, "%s %s %s %s", command, args1, args2, args3);

      if (strcmp(command, "quit") == 0) {
        sprintf(buffer, "quit");
        n = write(newsockfd, buffer, 255);
        if (n < 0) {
          printf("Error writing");
          exit(0);
        }
        exit(0);
      } else if (strcmp(command, "List") == 0) {
        // printf("B after entering the L function %s",buffer);
        int length = sprintf(buffer + 1, "%s", "TSLA | MSFT");
        buffer[0] = (char)length;
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
          printf("Error writing");
          exit(0);
        }
        // bzero(buffer, 255);

      } else if (strcmp(command, "Prices") == 0) {
        int found = 0;
        if (strcmp(args1, "TSLA") == 0) {
          for (int i = 0; i < sizeof(tslaData) / sizeof(tslaData[0]); i++) {
            if (strcmp(tslaData[i].date, args2) == 0) {
              // Date found, format the output
              int length =
                  sprintf(buffer + 1, "%.2lf", tslaData[i].closing_price);
              buffer[0] = (char)length;
              found = 1;
              break;
            }
          }

        } else if (strcmp(args1, "MSFT") == 0) {
          for (int i = 0; i < sizeof(msftData) / sizeof(msftData[0]); i++) {
            if (strcmp(msftData[i].date, args2) == 0) {
              // Date found, format the output
              int length =
                  sprintf(buffer + 1, "%.2lf", msftData[i].closing_price);
              buffer[0] = (char)length;
              found = 1;
              break;
            }
          }
        }
        if(checkdate(args2)==0){
          sprintf(buffer,"-Invalid Syntax");
        }
        else if (found != 1) {
          sprintf(buffer, "-Unknown");
        }
        
        n = write(newsockfd, buffer, 255);
        if (n < 0) {
          printf("Error writing");
          exit(0);
        }
        bzero(buffer, 255);
      } else if (strcmp(command, "MaxProfit") == 0) {
        float maxp = 0;
        int found=0;
        if (strcmp(args1, "TSLA") == 0) {
          int start = 0;
          int end = 0;
          for (int i = 0; i < sizeof(tslaData) / sizeof(tslaData[0]); i++) {
            if (strcmp(tslaData[i].date, args2) == 0) {
              start = i;
              break;
            }
          }
          for (int i = 0; i < sizeof(tslaData) / sizeof(tslaData[0]); i++) {
            if (strcmp(tslaData[i].date, args3) == 0) {
              end = i;
              break;
            }
          }

          for (int i = start; i <= end; i++) {
            for (int j = i + 1; j <= end; j++) {
              if ((tslaData[j].closing_price - tslaData[i].closing_price) >
                  maxp) {
                maxp = tslaData[j].closing_price - tslaData[i].closing_price;
                found=1;
              }
            }
          }

        }

        else if (strcmp(args1, "MSFT") == 0) {
          int start = 0;
          int end = 0;
          for (int i = 0; i < sizeof(msftData) / sizeof(msftData[0]); i++) {
            if (strcmp(msftData[i].date, args2) == 0) {
              start = i;
              break;
            }
          }
          for (int i = 0; i < sizeof(msftData) / sizeof(msftData[0]); i++) {
            if (strcmp(msftData[i].date, args3) == 0) {
              end = i;
              break;
            }
          }

          for (int i = start; i <= end; i++) {
            for (int j = i + 1; j <= end; j++) {
              if ((msftData[j].closing_price - msftData[i].closing_price) >
                  maxp) {
                maxp = msftData[j].closing_price - msftData[i].closing_price;
                found=1;
              }
            }
          }
        }
        int length = 0;
        if(checkdate(args2)==0 ||checkdate(args3)==0){
          length=strlen("-Invalid Syntax");
          sprintf(buffer,"-Invalid Syntax");
        }
        else if (found != 1) {
          length=strlen("Unknown");
          sprintf(buffer, "-Unknown");
        }
        else{
           length = sprintf(buffer + 1, "%.2lf", maxp);
          buffer[0] = (char)length;
        }
        n = write(newsockfd, buffer, length + 1);
        if (n < 0) {
          printf("Error writing");
          exit(0);
        }

        bzero(buffer, 255);

      } else {
        sprintf(buffer, "-Invalid Syntax");
        n = write(newsockfd, buffer, 255);
        if (n < 0) {
          printf("Error writing");
          exit(0);
        }
      }
      bzero(buffer, 255);
    }
  }
  close(sockfd);
  close(newsockfd);
  return 0;
}