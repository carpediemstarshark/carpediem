#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>

char* get_date()
{
   struct tm* tm;
   struct timeval tv;

   gettimeofday(&tv, NULL);
   tm = localtime(&tv.tv_sec);

   int year = tm->tm_year + 1900; //2019
   int month = tm->tm_mon + 1;    //12
   int day = tm->tm_mday;       //5

   static char yearnum[20];
   char monthnum[3];
   char daynum[3];

   char zero[2] = "0";
   char hyphen[2] = "-";

   sprintf(yearnum, "%d", year); //2019
   strcat(yearnum, hyphen);

   if (month < 10)
   {
      strcat(yearnum, zero); //20190
      sprintf(monthnum, "%d", month); //8
      strcat(yearnum, monthnum);  //201908
   }
   else
   {
      sprintf(monthnum, "%d", month); //12
      strcat(yearnum, monthnum); //201912
   }

   strcat(yearnum, hyphen);

   if (day < 10)
   {
      strcat(yearnum, zero); //2019120
      sprintf(daynum, "%d", day); //6
      strcat(yearnum, daynum); //20191206
   }
   else
   {
      sprintf(daynum, "%d", day); //22
      strcat(yearnum, daynum); //20191222
   }

   return yearnum;
}

int dir_list_print(char* dirpath){
   DIR* dir_info = NULL;
   struct dirent* dir_entry = NULL;

   dir_info = opendir(dirpath);

   if(dir_info == NULL){
      printf("ERR : dirrectory is not found\n");
      return -1;
   }

   while((dir_entry = readdir(dir_info)) != NULL){
      if(strcmp(dir_entry->d_name, "..")==0||strcmp(dir_entry->d_name,".")==0)
         continue;
      printf("%s\n",dir_entry->d_name);
   }
   closedir(dir_info);

   return 0;
}

char* makeFile(char* now) {

   int newfile; //만들 파일명
   static char path[30] = "./txt/";
   char text[10] = ".txt";
   //now을 시간값 입력받아서 now으로 파일생성
   char charnow[100];
   strcat(path, now);
   strcat(path, text);
   

   if (0 < (newfile = creat(path, 0644)))
   {
      close(newfile);
   }
   else
   {
      printf("ERR : file create err in makeFile\n");
   }

   return path;
}

void write_buf(char* file_name) {
   int fd, n, error;
   char buf[100];

   printf("%s\n", file_name);
   printf("please input data in diary.\n");
   __fpurge(stdin);
   gets(buf);
   n = strlen(buf);
   fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC);
   if (fd < 0) {
      printf("ERR : file open in write_buf error\n");
      exit(1);
   }
   error = write(fd, buf, n);
   if (error == -1) {
      printf("ERR : file write in write_buf error\n");
      exit(1);
   }
   close(fd);
}

void read_buf(char* file_name) {
   int open_diary, read_diary, n;
   char path[30] = "./txt/";
   char text[10] = ".txt";
   char buf[100];
   strcat(path, file_name);
   strcat(path, text);

   open_diary = open(path, O_RDWR); // 파일을 (읽기/쓰기) 용으로 연다 
   if (open_diary == -1) {
      printf("ERR : file open in read_buf error\n");
      exit(1);
   }

   read_diary = read(open_diary, buf, 100); //open_diary가 바라보는 파일을 buf에 100바이트씩 읽어옴
   if (read_diary == -1) {
      printf("ERR : file read in read_buf error\n");
      exit(1);
   }
   printf("%s\n%s\n", path, buf); //버퍼에 있는 값을 출력함
   close(open_diary);
}

int main(){
   char* dirpath = "./txt";
   int input = 1;

   while (input != 3) {
      system("clear");
      printf("diary menu\n1. write\n2. read\n3. exit\ninput : ");
      scanf("%d", &input);

      system("clear");

      if (input == 1) {
         write_buf(makeFile(get_date()));

         system("clear");
      }
      else if (input == 2) {
         char inputDate[15];

         dir_list_print(dirpath);
         printf("please input date to read : ");
         scanf("%s", inputDate);
         read_buf(inputDate);

         __fpurge(stdin);
         printf("please input any key to continue : ");
         scanf("%s", inputDate);

         system("clear");
      }
      else if (input != 3) {
         printf("please input in index.\n");
      }
   }

   return 0;
}