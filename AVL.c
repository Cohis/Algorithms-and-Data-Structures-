#include "stdio.h"
#include "stdlib.h"



typedef struct nodes {

    int value;
    int sub_height;

    struct nodes *right;
    struct nodes *left;

}node;

//vibecoded function, just to see how my tree looks
void print_tree(node *root, int level){

    if(root == NULL)
        return;

    // print right first
    print_tree(root->right, level + 1);

    // indentation
    for(int i = 0; i < level; i++)
        printf("│   ");

    // identify the node
    if(level != 0)
        printf("├── ");
    else
        printf("└── ");

    printf("%d (h=%d)\n", root->value, root->sub_height);

    // print left
    print_tree(root->left, level + 1);
}


int max (int a, int b){
    if(a == b)
        return a;
    
    if(a > b)
        return a;
    
    if(b > a)
        return b;
}


int height (node* root){

    if(root == NULL)
        return -1;

    root->sub_height = 1 + max(height(root->left), height(root->right));
    
    return root->sub_height;
}

node* smallest_val(node* root){

    node *current = root;

    while(current->left != NULL)
        current = current->left;

    return current;
}

//balance factor calculation:

int bal_factor(node* root){

    //factor = left height - right height

    if(root == NULL)
        return 0;
    
    
    int right_height;
    int left_height;

    if(root->left == NULL)
        left_height = -1; // the floor is 0, the floor is occupied by the last position
                          // this value is set to make the equation correct

    else
        left_height = root->left->sub_height;                          

    if(root->right == NULL)
        right_height = -1;

    else
        right_height = root->right->sub_height;


    return left_height - right_height;
}

node* right_rotation(node* root){

    //left child goes up, the old node goes down to the right
    //right child of the node that went up becomes the left child of the node that went down

    node* new_root = root->left;

    node* T2 = new_root->right;


    //old root goes down to the right

    new_root->right = root;

    //T2 becomes the left child of the old root

    root->left = T2;


    //update heights

    root->sub_height = 1 + max(height(root->right), height(root->left));
    
    new_root->sub_height = 1 + max(height(new_root->right), height(new_root->left));

    return new_root;

}



node* left_rotation(node* root){

    //right child goes up, the old node goes down to the left
    //the left child of the node that went up becomes the right child of the node that went down

    node* new_root = root->right;

    node* T2 = new_root->left;

    new_root->left = root;

    root->right = T2;

    //height update

    root->sub_height = 1 + max(height(root->left), height(root->right));

    new_root->sub_height = 1 + max(height(new_root->left), height(new_root->right));

    return new_root;
}


node* balance(node* root){

    int bf = bal_factor(root);

    //if the node leans left (bf > +1), we check bal_factor(root->left)
    //if the node leans right (bf < -1), we check bal_factor(root->right)

    if(bf > 1){ //leaning left

        int bf_left = bal_factor(root->left);

        if(bf_left == +1 || bf_left == 0){
            //LL - Simple Right Rotation

            return right_rotation(root);

        }

        else if(bf_left == -1){
            //LR - Left-Right Rotation

            //simple left rotation on the left child
            root->left = left_rotation(root->left);

            //right rotation on the root
            return right_rotation(root);

        }

    }

    else if (bf < -1){ //leaning right

        int bf_right = bal_factor(root->right);

        if(bf_right == -1 || bf_right == 0){

            //RR - Simple Left Rotation

            return left_rotation(root);
        }

        else if(bf == -2 && bf_right == 1){
            //RL - Right-Left Rotation

            //right rotation on the right child

            root->right = right_rotation(root->right);

            //left rotation on the root

            return left_rotation(root);
        }

    }

    return root;

}


//recursive remove:

node* remove_node(node* root, int value){

    if(root == NULL)
        return NULL;


    //recursively search for the value

    if(root->value > value)
        root->left = remove_node(root->left, value);

    else if(value > root->value)
        root->right = remove_node(root->right, value);

    
    //only exits recursion when the value is found

    //this only executes after the recursion unwinds
    else{

        //case 1: node is a leaf

        if(root->left == NULL &&
           root->right == NULL){

            free(root);
            return NULL;
           }

        
        //case 2: node has only a right child

        if(root->left == NULL &&
            root->right != NULL){

                node *temp = root->right;

                free(root);

                return temp; //parent now points to its child
            }

        //case 3: node has only a left child
        
        if(root->right == NULL && 
            root->left != NULL){

                node* temp = root->left;

                free(root);

                return temp;
            }

        
        //case 4: node has both children
        //it is replaced by the smallest successor

        node *temp = smallest_val(root->right); //temp stores the desired value

        root->value = temp->value; //the value takes the position replacing the old one


        //remove the original successor:
        root->right = remove_node(root->right, temp->value);

        root->sub_height = 1 + max(height(root->left), height(root->right));

        root = balance(root);

        return root;
    }

    //this handles the recursion unwinding:
    root->sub_height = 1 + max(height(root->left), height(root->right));

    root = balance(root);

    return root;
}

//recursive insert:

node* insert(node* root, int value){   

    if(root == NULL){

        node* new_node = malloc(sizeof(node));

        new_node->right = NULL;
        new_node->left = NULL;

        new_node->value = value;
        new_node->sub_height = 0;

        return new_node;
    }


    //search for position:

    if(value < root->value)
        root->left = insert(root->left, value);

    else if(value > root->value)
        root->right = insert(root->right, value);

    
        
    //ignore duplicates:
    else
        return root;


    //height update:

    root->sub_height = 1 + max(height(root->left), height(root->right));

    //calculate the balance factor; rotate if needed

    
    root = balance(root);

    return root;

}

int main(){

    int N;

    printf("Enter the number of values to be inserted: ");
    scanf("%d", &N);

    node *root = NULL;


    int temp;

    for(int i=0; i<N; i++){

        scanf("%d", &temp);
        root = insert(root, temp);
        
        printf("\n");

    }

    print_tree(root, 0);

    printf("Enter the number of nodes to be removed: ");

    scanf("%d", &N);

    for(int i=1; i<=N; i++){
        scanf("%d", &temp);

        root = remove_node(root, temp);

        printf("tree after removal #%d \n", i);

        print_tree(root, i);
    }


}
