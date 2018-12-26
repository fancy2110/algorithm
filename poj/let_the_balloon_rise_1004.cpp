/**
 * http://acm.hdu.edu.cn/showproblem.php?pid=1004
 */
#include <iostream>

struct leaf
{
    struct leaf *items[25] = {NULL};
    int count = 0;
};

int addItem(struct leaf *root, char const buffer[], int max, char target[])
{
    using namespace std;
    int i = 0;
    char charAt = '\0';
    struct leaf *node = root;

    while ((charAt = buffer[i++]) != '\0')
    {
        int index = charAt - 'a';
        struct leaf *next_node = node->items[index];
        if (next_node == NULL)
        {
            next_node = new struct leaf;
            node->items[index] = next_node;
        }
        node = next_node;
    }

    node->count++;
    if (node->count > max)
    {
        max = node->count;
        for (int i = 0; i < 16; i++)
        {
            target[i] = buffer[i];
        }
    }

    return max;
}

int main(int argc, char const *argv[])
{
    using namespace std;
    int lineCount = 0;
    cin >> lineCount;
    char buffer[16] = {'\0'};
    char topWord[16] = {'\0'};

    struct leaf *ROOT = new struct leaf;

    while (lineCount > 0)
    {
        int max = 0;
        while (lineCount > 0)
        {
            cin >> buffer;
            max = addItem(ROOT, buffer, max, topWord);
            lineCount--;
        }

        cout << topWord << endl;

        for (int i = 0; i < 16; i++)
        {
            topWord[i] = '\0';
        }
        cin >> lineCount;
    }
    return 0;
}
