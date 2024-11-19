#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 5
#define MAX_FILES 10
#define MAX_FILE_LENGTH 100

typedef struct {
    char name[20];
    char address[10];    // 文件的物理地址
    char protection[10]; // 保护码：只读（read-only）或读写（read-write）
    int length;          // 文件长度
    char content[MAX_FILE_LENGTH]; // 文件内容
} File;

typedef struct {
    char username[20];
    char password[20];
    File files[MAX_FILES];
    int file_count;
} User;

User users[MAX_USERS];
int user_count = 0;
User *current_user = NULL;

// 初始化用户数据
void init_users() {
    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "password");
    users[0].file_count = 0;

    strcpy(users[1].username, "qiyan");
    strcpy(users[1].password, "1234");
    users[1].file_count = 0;

    user_count = 2;
}

// 用户登录
void login() {
    char username[20], password[20];
    printf("输入用户名: ");
    scanf("%s", username);
    printf("输入密码: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            current_user = &users[i];
            printf("登录成功。欢迎, %s!\n", current_user->username);
            return;
        }
    }
    printf("用户名或密码错误。\n");
}

// 列出目录
void list_directory() {
    if (current_user == NULL) {
        printf("请先登录。\n");
        return;
    }

    printf("用户 %s 的目录:\n", current_user->username);
    printf("文件名\t物理地址\t保护码\t长度\n");
    for (int i = 0; i < current_user->file_count; i++) {
        File *file = &current_user->files[i];
        printf("%s\t\t%s\t\t%s\t\t%d\n",
               file->name, file->address, file->protection, file->length);
    }
}

// 创建文件
void create_file() {
    if (current_user == NULL) {
        printf("请先登录。\n");
        return;
    }

    if (current_user->file_count >= MAX_FILES) {
        printf("文件数量已达上限。\n");
        return;
    }

    File *file = &current_user->files[current_user->file_count];

    printf("输入文件名: ");
    scanf("%s", file->name);
    printf("输入保护模式 (read-only / read-write): ");
    scanf("%s", file->protection);
    printf("输入文件内容: ");
    getchar(); // 吃掉换行符
    fgets(file->content, MAX_FILE_LENGTH, stdin);
    file->content[strcspn(file->content, "\n")] = 0; // 去掉换行符

    file->length = strlen(file->content);
    sprintf(file->address, "file%d", current_user->file_count);

    current_user->file_count++;
    printf("文件 '%s' 创建成功。\n", file->name);
}

// 删除文件
void delete_file() {
    if (current_user == NULL) {
        printf("请先登录。\n");
        return;
    }

    char file_name[20];
    printf("输入要删除的文件名: ");
    scanf("%s", file_name);

    for (int i = 0; i < current_user->file_count; i++) {
        if (strcmp(current_user->files[i].name, file_name) == 0) {
            for (int j = i; j < current_user->file_count - 1; j++) {
                current_user->files[j] = current_user->files[j + 1];
            }
            current_user->file_count--;
            printf("文件 '%s' 删除成功。\n", file_name);
            return;
        }
    }
    printf("未找到文件 '%s'。\n", file_name);
}

// 读取文件
void read_file() {
    if (current_user == NULL) {
        printf("请先登录。\n");
        return;
    }

    char file_name[20];
    printf("输入要读取的文件名: ");
    scanf("%s", file_name);

    for (int i = 0; i < current_user->file_count; i++) {
        if (strcmp(current_user->files[i].name, file_name) == 0) {
            printf("文件 '%s' 的内容: %s\n", file_name, current_user->files[i].content);
            return;
        }
    }
    printf("未找到文件 '%s'。\n", file_name);
}

// 写入文件
void write_file() {
    if (current_user == NULL) {
        printf("请先登录。\n");
        return;
    }

    char file_name[20];
    printf("输入要写入的文件名: ");
    scanf("%s", file_name);

    for (int i = 0; i < current_user->file_count; i++) {
        if (strcmp(current_user->files[i].name, file_name) == 0) {
            if (strcmp(current_user->files[i].protection, "read-write") == 0) {
                printf("输入新的内容: ");
                getchar(); // 吃掉换行符
                fgets(current_user->files[i].content, MAX_FILE_LENGTH, stdin);
                current_user->files[i].content[strcspn(current_user->files[i].content, "\n")] = 0; // 去掉换行符
                current_user->files[i].length = strlen(current_user->files[i].content);
                printf("文件 '%s' 更新成功。\n", file_name);
            } else {
                printf("文件 '%s' 是只读的，无法修改。\n", file_name);
            }
            return;
        }
    }
    printf("未找到文件 '%s'。\n", file_name);
}

// 文件系统菜单
void menu() {
    int choice;
    while (1) {
        printf("\n文件系统命令:\n");
        printf("1. 登录\n");
        printf("2. 列出目录\n");
        printf("3. 创建文件\n");
        printf("4. 删除文件\n");
        printf("5. 读取文件\n");
        printf("6. 写入文件\n");
        printf("7. 退出\n");
        printf("输入你的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                list_directory();
                break;
            case 3:
                create_file();
                break;
            case 4:
                delete_file();
                break;
            case 5:
                read_file();
                break;
            case 6:
                write_file();
                break;
            case 7:
                printf("退出文件系统。再见！\n");
                return;
            default:
                printf("无效的选择。\n");
        }
    }
}

int main() {
    init_users();
    menu();
    return 0;
}
