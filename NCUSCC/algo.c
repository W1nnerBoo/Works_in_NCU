#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

clock_t start, end,start1,end1,start2,end2,start3,end3;
double time0,time1,time2,time3;

// 冒泡排序实现
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // 交换元素
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 堆化函数，用于维护堆的性质
void heapify(int arr[], int n, int i) {
    int largest = i; // 初始化为根节点
    int left = 2 * i + 1; // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 如果左子节点存在并且比根节点大
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // 如果右子节点存在并且比最大的节点大
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // 如果最大值不是根节点，交换并继续堆化
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        // 递归堆化子树
        heapify(arr, n, largest);
    }
}

// 堆排序算法
void heapSort(int arr[], int n) {
    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // 逐一提取元素
    for (int i = n - 1; i >= 0; i--) {
        // 移动当前根到末尾
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // 调整堆结构
        heapify(arr, i, 0);
    }
}



#define MAX_NODES 33333333 // 假设最多100个节点

// 节点结构体
typedef struct FibonacciNode {
    int key;  // 节点的键值
    int degree;  // 节点的度数
    struct FibonacciNode *parent;  // 指向父节点
    struct FibonacciNode *child;   // 指向子节点
    struct FibonacciNode *left;    // 指向左兄弟
    struct FibonacciNode *right;   // 指向右兄弟
    int mark;  // 标记位
} FibonacciNode;

// 斐波那契堆
typedef struct FibonacciHeap {
    FibonacciNode *min;  // 指向最小的节点
    int n;  // 堆中节点的数量
} FibonacciHeap;

// 静态内存池
FibonacciNode node_pool[MAX_NODES];
int node_count = 0;

//debug
void printHeap(FibonacciHeap *heap)
{
    if (heap->min == NULL)
    {
        printf("Heap is empty\n");
        return;
    }
    printf("Heap nodes: ");
    FibonacciNode *x = heap->min;
    do
    {
        printf("%d ", x->key);
        x = x->right;
    } while (x != heap->min);
    printf("\n");
}

// 模拟手动内存分配：从池中获取新节点
FibonacciNode* createNode(int key) {
    if (node_count >= MAX_NODES) {
        printf("内存池已满，无法分配新节点\n");
        return NULL;
    }

    FibonacciNode *node = &node_pool[node_count++];
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;

    // 调试信息：验证节点创建
    //printf("创建节点：%d，当前已使用节点数量：%d\n", key, node_count);

    return node;
}

// 创建斐波那契堆
FibonacciHeap* createHeap() {
    static FibonacciHeap heap;
    heap.min = NULL;
    heap.n = 0;
    return &heap;
}

// 将节点加入双向链表
void insertNode(FibonacciNode *node, FibonacciNode *minNode) {
    if (minNode == NULL) {
        node->left = node;
        node->right = node;
    } else {
        node->right = minNode;
        node->left = minNode->left;
        minNode->left->right = node;
        minNode->left = node;
    }
}

// 插入一个节点到斐波那契堆中
void insertHeap(FibonacciHeap *heap, int key) {
    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化，无法插入节点\n");
        return;
    }

    FibonacciNode *node = createNode(key);
    if (node == NULL) {
        printf("错误：无法创建新节点，内存分配失败\n");
        return;
    }

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        insertNode(node, heap->min);
        if (key < heap->min->key) {
            heap->min = node;
        }
    }
    heap->n++;

    // 调试信息：验证插入节点后堆的状态
  //  printf("插入节点：%d，当前最小节点：%d，堆中节点数量：%d\n", key, heap->min->key, heap->n);
}


// 将一个节点y连接到另一个节点x上
void link(FibonacciNode *y, FibonacciNode *x) {
    // 从根链表中移除y
    y->left->right = y->right;
    y->right->left = y->left;

    // 把y作为x的子节点
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
    }
    x->degree++;
    y->mark = 0;
}
//辅助函数
void fibHeapLink(FibonacciHeap *heap, FibonacciNode *y, FibonacciNode *x)
{
    // 将y链接为x的子节点
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL)
    {
        x->child = y;
        y->left = y->right = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
    y->mark = 0;
}

// v2.0
// 合并堆的根链表，确保每个度数唯一
void _consolidate(FibonacciHeap *heap)
{
    int D = (int)(log(heap->n) / log(2)) + 1;
    // FibHeapNode *A[D];
    //  动态分配 A 数组 Windows的Mingw不支持c99
    FibonacciNode **A = (FibonacciNode **)malloc(D * sizeof(FibonacciNode *));
    // FibHeapNode *A = (FibHeapNode *) malloc(sizeof(FibHeapNode *[D]));

    for (int i = 0; i < D; i++)
    {
        A[i] = NULL;
    }

    FibonacciNode *x = heap->min;
    int numRoots = 0;
    if (x != NULL)
    {
        numRoots++;
        x = x->right;
        while (x != heap->min)
        {
            numRoots++;
            x = x->right;
        }
    }
    while (numRoots > 0)
    {
        int d = x->degree;
        FibonacciNode *next = x->right;

        while (A[d] != NULL)
        {
            FibonacciNode *y = A[d]; // y与x度数相同
            if (x->key > y->key)
            { // 交换x与y
                FibonacciNode *temp = x;
                x = y;
                y = temp;
            }
            fibHeapLink(heap, y, x);
            A[d] = NULL;
            d++;
        }

        A[d] = x;
        x = next;
        numRoots--;
    }

    heap->min = NULL;

    for (int i = 0; i < D; i++)
    {
        if (A[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            }
            else
            {
                A[i]->left = heap->min;
                A[i]->right = heap->min->right;
                heap->min->right->left = A[i];
                heap->min->right = A[i];
                if (A[i]->key < heap->min->key)
                {
                    heap->min = A[i];
                }
            }
        }
    }
    // a small bug i forgot
    free(A);
}

// 度数合并操作
void consolidate(FibonacciHeap *heap) {
    int maxDegree = 20;  // 假设最大度数为20
    FibonacciNode *A[maxDegree];

    for (int i = 0; i < maxDegree; i++) {
        A[i] = NULL;
    }

    FibonacciNode *x = heap->min;
    FibonacciNode *start = heap->min;
    do {
        FibonacciNode *next = x->right;
        int d = x->degree;
        while (A[d] != NULL) {
            FibonacciNode *y = A[d];
            if (x->key > y->key) {
                FibonacciNode *temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        x = next;
    } while (x != start);

    heap->min = NULL;

    for (int i = 0; i < maxDegree; i++) {
        if (A[i] != NULL) {
            if (heap->min == NULL) {
                heap->min = A[i];
            } else {
                insertNode(A[i], heap->min);
                if (A[i]->key < heap->min->key) {
                    heap->min = A[i];
                }
            }
        }
    }

    // 调试信息：合并后的最小节点
    if (heap->min != NULL) {
        printf("合并后最小节点：%d\n", heap->min->key);
    } else {
        printf("合并后堆中没有最小节点\n");
    }
}

FibonacciNode *_extractMin(FibonacciHeap *heap)
{
    FibonacciNode *z = heap->min;

    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化\n");
        return NULL;
    }

    if (heap->min == NULL) {
        printf("错误：堆中没有节点可以提取\n");
        return NULL;
    }

    if (z != NULL)
    {
        // 将最小节点的每个子节点添加到根链表中
        if (z->child != NULL)
        {
            FibonacciNode *child = z->child;
            do
            {
                FibonacciNode *next = child->right;
                child->left = heap->min;
                child->right = heap->min->right;
                heap->min->right->left = child;
                heap->min->right = child;
                child = next;
            } while (child != z->child);
        }

        // 移除最小节点
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            heap->min = NULL;
        }
        else
        {
            heap->min = z->right;
            // 调整堆
            _consolidate(heap); // 需要实现合并堆
        }
        heap->n--;
        // 调试信息：验证提取最小节点后堆的状态
//        printf("提取最小节点：%d，当前最小节点：%d（如果存在），堆中剩余节点数量：%d\n",z->key, heap->min != NULL ? heap->min->key : -1, heap->n);
    }
    return z;
}


// 提取堆中的最小节点
FibonacciNode* extractMin(FibonacciHeap *heap) {
    if (heap == NULL) {
        printf("错误：斐波那契堆未初始化\n");
        return NULL;
    }

    if (heap->min == NULL) {
        printf("错误：堆中没有节点可以提取\n");
        return NULL;
    }

    FibonacciNode *z = heap->min;
    if (z != NULL) {
        // 把最小节点的孩子移到根链表
        if (z->child != NULL) {
            FibonacciNode *child = z->child;
            do {
                child->parent = NULL;
                child = child->right;
            } while (child != z->child);
        }

        // 从根链表中移除z
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            heap->min = NULL;
        } else {
            heap->min = z->right;
            //debug
            printHeap(heap);
            _consolidate(heap);  // 执行度数合并
        }

        heap->n--;
        //debug
        //printHeap(heap);
        // 调试信息：验证提取最小节点后堆的状态
        //printf("提取最小节点：%d，当前最小节点：%d（如果存在），堆中剩余节点数量：%d\n",z->key, heap->min != NULL ? heap->min->key : -1, heap->n);

        return z;
    }

    printf("错误：无法提取最小节点，未知原因\n");
    return NULL;
}


// 斐波那契堆排序算法
void fibonacciHeapSort(int arr[], int n) {
    FibonacciHeap *heap = createHeap();
    if (heap == NULL) {
        printf("错误：无法创建斐波那契堆\n");
        return;
    }

    // 插入所有元素到斐波那契堆
    for (int i = 0; i < n; i++) {
        insertHeap(heap, arr[i]);
    }

    //debug
    //printHeap(heap);
    //插入节点是正常的

    // 提取最小值进行排序
    for (int i = 0; i < n; i++) {
        FibonacciNode *minNode = _extractMin(heap);
        if (minNode != NULL) {
            arr[i] = minNode->key;
        } else {
            printf("错误：无法提取最小节点，排序失败\n");
            return;
        }
    }
}


void printResourceUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    printf("User CPU time used: %ld.%06ld sec\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System CPU time used: %ld.%06ld sec\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Maximum resident set size: %ld KB\n", usage.ru_maxrss); // 内存占用（最大常驻集大小）
    printf("Minor page faults: %ld\n", usage.ru_minflt);
    printf("Major page faults: %ld\n", usage.ru_majflt);
    printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
    printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
}


// 从文件读取数据到数组
int* readTestData(const char *filename, int *numData) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件！\n");
        return NULL;
    }

    int *data = (int*)malloc(sizeof(int) * (*numData));
    if (data == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }

    // 读取数据
    for (int i = 0; i < *numData; i++) {
        fscanf(file, "%d", &data[i]);
    }

    fclose(file);
    return data;
}




int main() {
    const char *filename = "test_data.txt";  // 之前生成的数据文件
    int numData = 100000;  // 10万条数据

start = clock();
start1 = clock();

    // // 读取测试数据
     int *data = readTestData(filename, &numData);
     if (data == NULL) {
         return -1;
     }

     printf("成功读取 %d 条数据\n", numData);

    // // 排序前显示部分数据（可选）
     printf("排序前的数据：\n");
     printArray(data, 20);  // 打印前10个数据

    // // 进行冒泡排序
     bubbleSort(data, numData);

    // // 排序后显示部分数据（可选）
     printf("排序后的数据：\n");
     printArray(data, 20);  // 打印前10个排序后的数据
    
end1 = clock();
time1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

printf("Bubble排序执行时间：%f 秒\n", time1);

     free(data);    
start2 = clock();
     data = readTestData(filename, &numData);
     if (data == NULL) {
         return -1;
     }

     printf("成功读取 %d 条数据\n", numData);

    // // 排序前显示部分数据（可选）
     printf("排序前的数据：\n");
     printArray(data, 20);  // 打印前10个数据

    // // 进行堆排序
     heapSort(data, numData);

    // // 排序后显示部分数据（可选）
     printf("排序后的数据：\n");
     printArray(data, 20);  // 打印前10个排序后的数据

end2 = clock();
time2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;   
printf("Heap排序执行时间：%f 秒\n", time2);
     free(data);

    //斐波那契堆排序
    start3=clock();
     data = readTestData(filename, &numData);
    //int data[10] = {1,5,6,77,98,103,23,435,654,3243};
    if (data == NULL) {
        return -1;
    }

    //printf("成功读取 %d 条数据\n", numData);

    // 排序前显示部分数据（可选）
    printf("排序前的数据：\n");
    printArray(data, 10);  // 打印前10个数据

    // 进行fibonacciHeap排序
    fibonacciHeapSort(data, numData);
    // 排序后显示部分数据（可选）
    printf("排序后的数据：\n");
    printArray(data, 20);  // 打印前10个排序后的数据
    free(data);
end3 = clock();
end = clock();
time3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
time0= ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Fibb排序执行时间：%f 秒\n", time3);
printf("All排序执行时间：%f 秒\n", time0);

 printResourceUsage();
return 0;


}

