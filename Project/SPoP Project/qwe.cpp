#include <iostream>
#include <string.h>
#include <time.h>

using namespace std;
struct Node
{
    char data[65];//key
    Node *parent;//parent node
    Node *left;//left children
    Node *right;//right children
    char value[257];//value
    int valuesize;
    int keysize;
    int color;//color of node
    int num;
};
typedef Node *NodePtr;

struct Slice
{
    int size;
    char *data;
};

struct Mempiece
{
    Node *root;
    int used;
} mempiece;
int NUMBER = 100;//bulk memory
int lextree[52][53];//count for all possible first 2 combination of keys 
int lexhash[52];//count of fisrt letter
class kvStore
{
private:
    NodePtr root[52][53];// root nodes of each tree
    NodePtr TNULL;//null node
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        node->parent = parent;
        node->left = nullptr;//NodeULL
        node->right = nullptr;
        node->color = 0;
        node->num=0;
    }

    void inOrderHelper(NodePtr node, int k, int *j, char *w, char *v)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left, k, j, w, v);
            (*j)++;
            if (*j == k)
            {
                strcpy(w, node->data);
                strcpy(v, node->value);
                // w.size = node->datasize;
                // v.size = node->valuesize;
                return;
            }
            inOrderHelper(node->right, k, j, w, v);
        }
    }

    NodePtr searchTreeHelper(NodePtr node, char key[])
    {
        int l = strcmp(key, node->data);
        if (node == TNULL || l == 0)
        {
            return node;
        }
        if (l < 0)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }
    void deleteFix(NodePtr x, int i, int j)
    {
        NodePtr s;
        while (x != root[i][j] && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s == TNULL)
                {
                    break;
                }
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent, i, j);
                    s = x->parent->right;
                }
                else if (s->left->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == 0)
                    {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s, i, j);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent, i, j);
                    x = root[i][j];
                }
            }
            else
            {
                s = x->parent->left;
                if (s == TNULL)
                {
                    break;
                }
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent, i, j);
                    s = x->parent->left;
                }
                else if (s->right->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s, i, j);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent, i, j);
                    x = root[i][j];
                }
            }
        }
        x->color = 0;
    }
    void rbTransplant(NodePtr u, NodePtr v, int i, int j)
    {
        if (u->parent == nullptr)
        {
            root[i][j] = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    bool deleteNodeHelper(NodePtr node, char key[], int i, int j)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL)
        {
            if (strcmp(node->data, key) == 0)
            {
                z = node;
                break;
            }
            if (strcmp(node->data, key) <= 0)
            {
                node->num--;
                node = node->right;
            }
            else
            {
                node->num--;
                node = node->left;
            }
        }
        if (z == TNULL)
        {
            return false;
        }
        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL)
        {
            x = z->right;
            rbTransplant(z, z->right, i, j);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rbTransplant(z, z->left, i, j);
        }
        else
        {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                rbTransplant(y, y->right, i, j);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y, i, j);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            y->num = z->num-1;
        }
        //delete z;
        if (y_original_color == 0)
        {
            deleteFix(x, i, j);
        }
        return true;
    }
    void insertFix(NodePtr k, int i, int j)
    {
        NodePtr uncle;
        while (k->parent->color == 1)
        {
            if (k->parent == k->parent->parent->right)//right case
            {
                uncle = k->parent->parent->left;
                if (uncle->color == 1)//no change
                {
                    uncle->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)//right left case
                    {
                        k = k->parent;
                        rightRotate(k, i, j);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent, i, j);
                }
            }
            else//left case
            {
                uncle = k->parent->parent->right;
                if (uncle->color == 1)//no change
                {
                    uncle->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)//left right case
                    {
                        k = k->parent;
                        leftRotate(k, i, j);
                    }
                    k->parent->color = 0;//left left case 
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent, i, j);

                }
            }
            if (k == root[i][j])
            {
                break;
            }
        }
        root[i][j]->color = 0;
    }

public:
    kvStore(uint64_t max_entries)
    {
        
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        TNULL->num =0;
        for (int i = 0; i < 52; i++)
            for (int j = 0; j < 53; j++)
                root[i][j] = TNULL;
    }

    bool get(Slice &key, Slice &value)
    {
        int lenght;
        if (key.data[0] <= 'Z')
            lenght = key.data[0] - 'A';
        else
            lenght = key.data[0] - 'a' + 26;
        int index;
        if (key.data[1] == 0)
            index = 0;
        else if (key.data[1] <= 'Z')
            index = key.data[1] - 'A' + 1;
        else
            index = key.data[1] - 'a' + 27;
        NodePtr ptr = searchTreeHelper(this->root[lenght][index], key.data);
        if (ptr == TNULL)
            return false;
        else
        {
            value.data = ptr->value;
            value.size = ptr->valuesize;
            key.size = ptr->keysize;
            // cout << ptr->value << endl;
            // cout << value.data << endl;
            // cout << value.size <<endl;
            // cout << ptr->valuesize << endl;
            return true;
        }
    }
    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
            node->num--;
            node = node->left;
        }
        return node;
    }

    void leftRotate(NodePtr x, int i, int j)
    {
        // x is grand parent
        // cout << "leeft" << endl;
        int q = x->left->num;//left of g
        int w = x->right->num; //right of g
        
        NodePtr y = x->right;
        
        int e = y->left->num;//left of p
        if(y->left==TNULL)
            e=0;
        int r = y->right->num; // right of p

        x->right = y->left;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[i][j] = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
        x->num = q+e+1;
        y->num = x->num + r + 1;
    }
    void rightRotate(NodePtr x, int i, int j)
    {
        // x is grand parent
        // cout << "right" << endl;
        int q = x->left->num;//left of g
        int w = x->right->num;//right of g
        
        NodePtr y = x->left;//parent
        
        int e = y->right->num; // right of p
        if(y->right==TNULL)
            e=0;
        int r = y->left->num; // left of p

        x->left = y->right;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[i][j] = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
        x->num = w+e+1;
        y->num = r+x->num+1;
    }
    bool decCount(Slice &key,int len1,int len2)
    {
        NodePtr x = this->root[len1][len2];
        while (x != TNULL)
        {
            int l = strcmp(key.data, x->data);
            if (l == 0)
            {
                return true;
            }
            if (l < 0)
            {
                x->num--;
                x = x->left;
            }
            else
            {
                x->num--;
                x = x->right;
            }
        }
        return true;
    }
    bool put(Slice &key, Slice &value)
    {
        int lenght;
        if (key.data[0] <= 'Z')
            lenght = key.data[0] - 'A';
        else
            lenght = key.data[0] - 'a' + 26;
        int index;
        if (key.data[1] == 0)
            index = 0;
        else if (key.data[1] <= 'Z')
            index = key.data[1] - 'A' + 1;
        else
            index = key.data[1] - 'a' + 27;
        lextree[lenght][index]++;
        lexhash[lenght]++;
        if (mempiece.root == NULL || mempiece.used == NUMBER)
        {
            mempiece.root = new Node[NUMBER];
            mempiece.used = 0;
        }
        NodePtr node = &mempiece.root[mempiece.used++];
        node->parent = nullptr;
        strcpy(node->data, key.data);
        
        strcpy(node->value, value.data);
        // cout << node->value << endl;
        // cout << value.size << endl;
        node->valuesize=value.size;
        node->keysize=key.size;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;
        node->num=1;
        NodePtr y = nullptr;
        NodePtr x = this->root[lenght][index];
        while (x != TNULL)
        {
            y = x;
            int l = strcmp(node->data, x->data);
            if (l == 0)
            {
                strcpy(x->value, value.data);
                x->valuesize = value.size;
                lextree[lenght][index]--;
                lexhash[lenght]--;

                decCount(key,lenght,index);
                
                return true;
            }
            if (l < 0)
            {
                x->num++;
                x = x->left;
            }
            else
            {
                x->num++;
                x = x->right;
            }
        }
        node->parent = y;
        if (y == nullptr)
        {
            root[lenght][index] = node;
        }
        else if (strcmp(node->data, y->data) < 0)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }
        if (node->parent == nullptr)
        {
            node->color = 0;//0 for black
            return false;//DOUBT return false kyo kiya he
        }
        if (node->parent->parent == nullptr)
        {
            return false;//WHY ye condition kyo zaruri he
        }
        insertFix(node, lenght, index);
        return false;//return false kyo kar raha he
    }
    bool get(int n, Slice &key, Slice &value)
    {
        int i = 0;
        int temp = 0;
        int f = 0;
        for (; i < 52; i++)
        {
            if (n <= temp + lexhash[i])
            {
                f = 1;
                break;
            }
            temp += lexhash[i];
        }
        // cout << temp+lexhash[i] << endl;
        if (f == 0)
            return false;
        int j = 0;
        for (; j < 53; j++)
        {
            if (n <= temp + lextree[i][j])
                break;
            temp += lextree[i][j];
        }

        
        int k = n - temp;
        NodePtr rot = this->root[i][j];
        // int yu;
        while(rot!=TNULL)
        {
            // if (rot == TNULL)
            // {
            //     return false;
            // }
            // if(rot->right == TNULL)
            //     yu=0;
            // else
            //     yu=  rot->right->num;

            if((rot->num - rot->right->num)== k)
            {
                strcpy(key.data,rot->data);
                strcpy(value.data,rot->value);
                value.size = rot->valuesize;
                key.size = rot->keysize;
                // cout << "main   " << rot->data << endl;
                // cout << "main   " << key.data << endl;
                return true;
            }
            else if(rot->left->num>=k)
                rot = rot->left;
            else
            {
                k = k-rot->left->num-1;
                rot=rot->right;                
            }
        }
        //  int *k = new int;
        // *k = 0;
        // inOrderHelper(this->root[i][j], n - temp, k, key.data, value.data);
        return true;
    }
    
    bool del(Slice &key)
    {
        int lenght;
        if (key.data[0] <= 'Z')
            lenght = key.data[0] - 'A';
        else
            lenght = key.data[0] - 'a' + 26;
        int index;
        if (key.data[1] == 0)
            index = 0;
        else if (key.data[1] <= 'Z')
            index = key.data[1] - 'A' + 1;
        else
            index = key.data[1] - 'a' + 27;
        if (deleteNodeHelper(this->root[lenght][index], key.data, lenght, index) == true)
        {
            lextree[lenght][index]--;
            lexhash[lenght]--;
            return true;
        }
        else
            return false;
    }
    bool del(int n)
    {
        Slice key, value;
        key.data = new char[65];
        value.data = new char[257];
        bool success = get(n, key, value);
        if (!success)
            return false;
        del(key);
        return true;
    }
    
};
// int main()
// {
//     kvstore bst;
//     int n;
//     cin >> n;
//     // char key[64];
//     // char value[256];
//     clock_t s, e;
//     double t=0;

//     Slice key,value;
//     key.data = new char[65];
//     value.data = new char[257];
//     for (int i = 0; i < n; i++)
//     {
//         cin >> key.data >> value.data;
//         s = clock_gettime();
//         bst.put(key, value);
//         e = clock_gettime();
//         t+=((double)e-(double)s); 
//     }
//     t=t/CLOCKS_PER_SEC;
//     printf("time_taken = %f\n",t);
//     //bst.printTree();
//     // cout << endl
//     //      << "After deleting" << endl;
//     // //bst.deleteNode(13);
//     // bst.printTree();
//     return 0;
// }
