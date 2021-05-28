#include<fstream>
#include <iostream>
#include <cmath>

#define ll long long
using namespace std;
ifstream fin("arbint.in");
ofstream fout("arbint.out");

struct node {
    node *parent; // Parent pointer
    node *child; // Child pointer
    node *left; // Pointer to the node on the left
    node *right; // Pointer to the node on the right
    int key; // Value of the node
    int degree; // Degree of the node
};

void swap(node *node1, node *node2) {
    node *aux;
    aux = node1;
    node1 = node2;
    node2 = aux;
}

node* min(node *node1, node *node2){
    if(node1->degree < node2->degree) return node1;
    else return node2;
}

node *root = NULL;
unsigned nrNoduri = 0;

void insertion(int val) {
    node *nodCurent = new node;
    nodCurent->key = val;
    nodCurent->degree = 0;
    nodCurent->parent = NULL;
    nodCurent->child = NULL;
    nodCurent->left = nodCurent;
    nodCurent->right = nodCurent;
    if (root == NULL) root = nodCurent;
    else {
        (root->left)->right = nodCurent; //inserand nodul in stanga radacinii, trebuie ca nodului curent din stanga
        //radacinii sa ii fac link cu noul nod inserat
        nodCurent->left = root->left;    //iar nodul curenet il link-uiesc la stanga cu nodul vechi din stanga radacinii
        nodCurent->right = root;         //nodul curent il leg la dreapta de radacina
        root->left = nodCurent;          //iar radacina o leg la stanga de nodul curent
        if (root->key < nodCurent->key)
            root = nodCurent;            //daca valoarea din nod este mai mica decat cea din radacina, nodul curent devine
        //radacina
    }
    nrNoduri++;
}

void link(node *node1, node *node2) {
    //tai legatura dintre node2 si vecinii sai, iar pe acestia in leg intre ei
    (node2->left)->right = node2->right;
    (node2->right)->left = node2->left;

    if(node1->right == node1) root = node1; // posibil in urma link-uirii ca nodul curent sa ramana singurul nod din lista de radacini

    //initializez legaturile
    node2->left = node2;
    node2->right = node2;
    node2->parent = node1;

    if(node1->child == NULL) node1->child = node2;
    //inseram noul nod la stanga lui child
    node2->right = node1->child;
    node2->left = (node1->child)->left; //node 2 se leaga la stanga cu nodul din stanga lui child
    ((node1->child)->left)->right = node2; //nodul vechi din stanga lui child se leaga la dreapta cu noul nod
    (node1->child)->left = node2; //iar child se leaga la stanga cu noul nod
    if(node2->key < (node1->child)->key)
        node1->child = node2;
    node1->degree ++;
}

void merge() {
    int lngDegree = int((log(nrNoduri)) / (log(2)));
    node *degrees[lngDegree]; // creez un vector cu toate marimile de forma 2ˆk ale arborilor aflati in heap
    //poate fi considerat un vector de frecventa, la pozitia i avem un arbore cu 2ˆi noduri
    for (int i = 0; i <= lngDegree; i++)
        degrees[i] = NULL;

    node *nodCurent = root;
    node *nodGasitAnterior;
    int currentDegree;
    do {
        currentDegree = nodCurent->degree; //pentru fiecare nod din lista de radacini
        while (degrees[currentDegree] != NULL) { //verific daca am gasit inainte un nod cu acelasi grad ca al sau
            //in caz afirmativ iau nodul de pe pozitia currentDegree gasit anterior si ii fac link cu noul nod gasit
            nodGasitAnterior = degrees[currentDegree];
            if(nodCurent->key > nodGasitAnterior->key) //verific valoarea din nodul curent este mai mare decat cea din nodul gasit anterior
                swap(nodCurent, nodGasitAnterior); //in caz afiramtiv, interschimb cele 2 noduri, valoarea minima fiind in nodCurent
            else if(nodGasitAnterior == root) root = nodCurent; //in caz negativ (adica valoarea din nodul curent este deja mai mica
            //decat cea din nodul gasit anterior, verific daca nodul gasit anterior este radacina, daca da, radacina devine nodul curent
            link(nodCurent, nodGasitAnterior); //fac link intre cele 2 noduri
            if(nodCurent->right == nodCurent) root = nodCurent; // posibil in urma link-uirii ca nodul curent sa ramana singurul nod din lista de radacini
            degrees[currentDegree]= NULL; // nu mai avem nimic gasit anterior pe pozitia respectiva, pentru ca am facut merge celor 2 arbori
            currentDegree ++; // deoarece in urma link-uirii degree-ul nodului curent a crescut, asa ca inaintam pe urmatoarea pozitie
            //in vectorul de frecventa
        }

        degrees[currentDegree] = nodCurent; //salvez in vectorul de frecventa
        nodCurent = nodCurent->right; //trec la urmatorul nod din dreapta
    } while (nodCurent != root);

    //in urmatoarea secventa for adaug toate nodurile ramase in vectorul de frecventa la lista de noduri
    //mai precis leg fiecare nod ramas in lista de radacini
    root = NULL;
    for(int i = 0; i <= lngDegree; i ++){
        if(degrees[i] != NULL){
            degrees[i]->left = degrees[i];
            degrees[i]->right = degrees[i];

            //nodurile se adauga unul cat09oe unul la stanga radacinii
            if(root != NULL){
                (root->left)->right = degrees[i]; // nodul vechi din stanga radacinii se leaga la dreapta de noul nod
                degrees[i]->right = root; //noul nod se leada la dreapta de radacina
                degrees[i]->left = root->left; //noul nod se leaga la stanga cu vechiul nod din stanga radacinii
                root->left = degrees[i]; //iar in final radacina se leaga la stanga cu noul nod
            } else if(root == NULL) root = degrees[i]; // dac
            else if(root->key > degrees[i]->key) root = degrees[i];
        }
    }
}

void delete_min(){
    if(root == NULL) cout << "Heap-ul este gol." << '\n';
    else {
        node* rootCopy = root;
        node* childRoot;
        node* childBrotherRoot;
        if(rootCopy->child != NULL){ //daca radacina are copii
            childRoot = rootCopy->child; //iau fiecare copil in parte

            //trecem prin toti copiii radacinei si ii adaugam la lista de radacini
            //mai exact le fac legaturi cu vecinii radacinei din care fac parte
            do{
                childBrotherRoot = childRoot->right; //salvez fratele lui child

                (root->left)->right = childRoot; // leg vecinul din dreapta radacinii la stanga cu copilul acesteia
                childRoot->right = root; //leg copilul la dreapta de radacina
                childRoot->left = root->left; //si la stanga de vecinul initial din stanga al radacinii
                root->left = childRoot; //leg la stanga radacina de copil

                childRoot->parent = NULL; //elimin legatura de parinte din componenta copilului
                childRoot = childBrotherRoot; //si trec la urmatorul copil al radacinii
            }while (childBrotherRoot != rootCopy->child); //fac acest proces pentru fiecare copil al radacinii, ne oprim
                                                        //cand ajungem din nou pe copilul cu care am inceput interatia
        }

        // leg vecinii radacinii unul de celalalt (practic tai legaturile dintre radacinia si vecinii sai)
        (rootCopy->left)->right = rootCopy->right; //cel din stanga il leg la dreapta cu vecinul din dreapta radacinii
        (rootCopy->right)->left = rootCopy->left;  //iar pe cel din dreapta il leg la stanga cu vecinul din stanga radacinii

        root = rootCopy->right; //radacina devine vecinul sau din dreapta (practic se sterge)

        //tratam cazul in care in heap se afla doar un singur nod
        if(rootCopy == rootCopy->right and rootCopy->child == NULL) root = NULL; //in caz afirmativ, acest se elimina
        else  merge(); //in caz negativ, facem consolidate pentru toti arborii aflati in heap, acesta se opreste cand
                        //toti arborii au un degree diferit

        nrNoduri--;
    }
}

// Function to display the heap
void display()
{
    node* rootCopey = root;
    if (rootCopey == NULL) cout << "Heap-ul este gol" << endl;
    else {
        cout << "Nodurile din heap sunt: " << endl;
        do {
            cout << rootCopey->key;
            rootCopey = rootCopey->right;
            if (rootCopey != root) cout << "-->";
        } while (rootCopey != root && rootCopey->right != NULL);
        cout << endl << "Heap-ul are " << nrNoduri << " noduri" << endl << endl;
    }
}

int main() {
    insertion(2);
    insertion(24);
    insertion(3);

    display();

    cout << "Miimul s-a extras" << endl;
    delete_min();
    display();
}