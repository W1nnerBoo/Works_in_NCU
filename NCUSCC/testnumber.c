#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 生成随机整数测试数据并写入文件
void generateTestData(int numData, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件！\n");
        return;
    }

    // 使用当前时间作为随机数种子
    srand(time(NULL));

    // 生成numData条随机数据
    for (int i = 0; i < numData; i++) {
        // 生成一个随机整数
        int randomInt = rand() % 1000000;  // 范围为0到999999的随机整数
        fprintf(file, "%d\n", randomInt);  // 将数据写入文件
    }

    fclose(file);
    printf("生成 %d 条数据到文件：%s\n", numData, filename);
}

int main() {
    int numData = 100000;  // 生成10万条数据
    const char *filename = "test_data.txt";  // 保存到的文件名

    generateTestData(numData, filename);
    
    return 0;
}

