/*
打开一个已经存在的文件（例如src_file），使用只读方式；然后打开一个新建文件（例如dest_file），使用只写方式，新建文件的权限设置如下：
   文件所属者拥有读、写、执行权限；
   同组用户与其他用户只有读权限。
从src_file文件偏移头部500个字节位置开始读取1K byte字节数据，然后将读取出来的数据写入到dest_file文件中，
从文件开头处开始写入，1K byte字节大小，操作完成之后使用close显式关闭所有文件，然后退出程序。
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ASSERT_FILE(file) do{if((file) == NULL){ perror(""); exit(errno);}}while (0)
#define ASSERT(x) do{if((x) < 0){ perror(""); exit(errno);}}while (0)

int main(int argc, char *argv[])
{
    FILE *file1, *file2;
    int retval;
    unsigned char buf[1024];

    file1 = fopen(argv[1],"r");
    ASSERT_FILE(file1);
    file2 = fopen(argv[2],"w+");
    ASSERT_FILE(file2);

    retval = fseek(file1, 500, SEEK_SET);
    // ASSERT(retval);
    if (feof(file1)) 
    { 
        /* 到达文件末尾 */
        perror("end of file\n");
        exit(errno);
    }

    retval = fread(buf, 1,sizeof(buf),file1);
    // ASSERT(retval);
    if (ferror(file1)) 
    { 
        /* 发生错误 */ 
        perror("error:\n");
        //exit(errno);
        clearerr(file1); //清除标志
    } 
    else 
    {
         /* 未发生错误 */ 
    }

    retval = fwrite(buf, 1,sizeof(buf),file2);
    ASSERT(retval);

    retval = fclose(file1);
    ASSERT(retval);
    retval = fclose(file2);
    ASSERT(retval);
    exit(0);
}
