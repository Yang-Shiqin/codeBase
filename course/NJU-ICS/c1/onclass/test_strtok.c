#include <string.h>
#include <stdio.h>
 
int main () {
   char str[80] = "This is - www.runoob.com - website";
   const char s[2] = "-";
   char *token;
   
      printf( "sd %s\n", str );
   /* 获取第一个子字符串 */
   token = strtok(str, s);
   
      printf( "sd %s\n", str );
   /* 继续获取其他的子字符串 */
   while( token != NULL ) {
      printf( "%s\n", token );
      printf( "sd %s\n", str );
    
      token = strtok(NULL, s);
   }
   return(0);
}
