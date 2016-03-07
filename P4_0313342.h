//CodeBlocks
#include <iostream>
using namespace std;

template <class K,class E>
class BST;
//////////////////////////BSTNode//////////////////////////////////
template <class K,class E>
class BSTNode
{
private:
    K key;              //exponent
    E element;          //coefficient
    BSTNode *left;
    BSTNode *right;
public:
    friend class BST<K,E>;
    friend class PolyBST;
    BSTNode( const K & Key,const E & theElement) //construtor 有值的初始
    :left( NULL ), right( NULL ), key(Key), element(theElement) {}
};
/////////////////////////////////BST////////////////////////////////////
template <class K,class E>
class BST
{
public:
    BST():root(NULL) {};                    //constructor
    BST(const BST& r):root(NULL)
    {
        *this=r;   //copy constructor
    }
    void makeEmpty()
    {
        makeEmpty(root);   //delete all(driver)
    }
    void makeEmpty( BSTNode<K,E> * & t ) const;     //(horse)
    BSTNode<K,E>* Search(const K& k);
    void Insert(const K& k, const E& e);
    void Delete(int& value);

    BSTNode<K,E>* root;
};
///////////////////////////////PolyBST////////////////////////////////////
class PolyBST:public BST<int ,double>
{
public:
    PolyBST():BST() {}  //constructor
    ~PolyBST() {}
    void setTerm(const int& k, const double& e)
    {
        if(e==0&&Search(k)) //delete a term (input coef is zero)
        {
            int Key=k;
            Delete(Key);
        }
        Insert(k,e);
    }

    friend ostream& operator<<(ostream& out,PolyBST& b);    //Operator << (to an ostream) for printing out the polynomial.
    void inorderOut(BSTNode<int ,double>* bi,ostream& out,int& count);  //RVL的遞迴，可由最大的key輸出到最小 (用來支援Operator <<)

    void makeEmpty(BSTNode<int ,double>* a);    //清空BST
    void operator=(const PolyBST& b);           // Operator = for assigning one polynomial to another.
    void inorderAssignment(BSTNode<int ,double>* bi);    //中序遞迴assignment每個Node (用來支援Operator =)

    PolyBST operator+(const PolyBST& b);                    //Operator + for adding two polynomials
    void inorderAdd(PolyBST& c,BSTNode<int ,double>* bi);   //遞迴中序走訪b，將b的Node加入c中 (用來支援Operator + )

    PolyBST operator-(const PolyBST& b);                    //Operator – for subtracting one polynomial from another.
    void inorderSub(PolyBST& c,BSTNode<int ,double>* bi);   //遞迴中序走訪b，將b的Node加入*this中 (用來支援Operator - )

    PolyBST operator*(const PolyBST& b);                    //Operator * for multiplying two polynomials.
    void inorderMutiPolyA(PolyBST& c, BSTNode<int ,double>* bi);//中序遞迴b，再遞迴a的Node相乘(用來支援Operator * )
    void inorderMutiPolyB(PolyBST& c, BSTNode<int ,double>* bi,BSTNode<int ,double>* ai);//遞迴a的Node相乘(用來支援Operator *(poly))

    PolyBST operator*(const double& b);                     //Operator * for multiplying a polynomial and a double scalar.
    void inorderMutiDouble(PolyBST& c,BSTNode<int ,double>* ai,const double& b);    //(用來支援Operator * (double))

};
//////////////////////////////BSTNode 成員函示/////////////////////////////

///////////////Search///////////////////
template <class K, class E>
BSTNode<K,E>* BST<K,E>::Search(const K& k)
{
    BSTNode<K,E> *t = root;
    while (t)
    {
        if (k == t->key) return t; // found
        t = (k > t->key) ? t->right : t->left;
    }
    return 0;
}

///////////////Insert///////////////////
template <class K, class E>
void BST<K,E>::Insert(const K& k, const E& e)
{
    BSTNode<K,E> *t = root, *p = 0;
    while (t)
    {
        if (k == t->key)    // duplicate key
        {
            t->element = e;
            return; // just update element
        }
        p = t;  // remember the parent of next t
        t = (k > t->key) ? t->right : t->left;
    }
    // insert a node
    t = new BSTNode<K,E>(k,e);
    if (root)   // tree not empty
        if (k < p->key) p->left = t;
        else p->right = t;
        else root = t;
}

/////////////////
//Delete a node//
/////////////////
template <class K, class E>
void BST<K,E>::Delete(int& Key)
{
    if(root==NULL)  //若為空tree
        return ;
    BSTNode<K,E> *t = root,*p=0;
    if(Search(Key)==root)   //若要delete root
    {
        if(t->left==NULL&&t->right==NULL)   //若只有一Node
        {
            delete t;
            root=NULL;
            return;
        }
        if(t->left==NULL&&t->right)         //若只有右子樹
        {
            root=t->right;
            delete t;
            return;
        }
        if(t->left&&t->right==NULL)         //若只有左子樹
        {
            root=t->left;
            delete t;
            return;
        }
        if(t->left&&t->right)               //若有兩個子樹
        {
            //choose the rightmost node in its left subtree
            BSTNode<K,E> * temp,*rMost=t->left;
            temp=t;
            while(rMost->right)
            {
                temp=rMost;         //temp=下一個rMost的parent
                rMost=rMost->right;
            }
            if(rMost->left)
                temp->right=rMost->left;
            rMost->left=root->left;
            rMost->right=root->right;
            delete root;
            root=rMost;
            return;
        }
    }
    while (t)
    {
        if (Key == t->key)      // duplicate key
        {
            break;
        }
        p = t;                  // remember the parent of next t
        t = (Key > t->key) ? t->right : t->left;
    }

    if(t==NULL)
        cout<<"the node doesn't exist!"<<endl;
    else if(t->right==NULL&&t->left==NULL)  //leaf node
    {
        if(p->key>t->key)       //t為p的left
        {
            p->left=NULL;
            delete t;
        }
        else                    //t為p的right
        {
            p->right=NULL;
            delete t;
        }
        return;
    }
    else if(t->right==NULL||t->left==NULL)  //has only one child
    {
        if(t->right)
        {
            if(p->key>t->key)   //t為p的left
            {
                p->left=t->right;
                delete t;
            }
            else                //t為p的right
            {
                p->right=t->right;
                delete t;
            }
        }
        else if(t->left)
        {
            if(p->key>t->key)   //t為p的left
            {
                p->left=t->left;
                delete t;
            }
            else                //t為p的right
            {
                p->right=t->left;
                delete t;
            }
        }
    }
    else if(t->right!=NULL&&t->left!=NULL)  //has two children
    {
        //choose the rightmost node in its left subtree
        BSTNode<K,E> * temp,*rMost=t->left;
        temp=t;
        while(rMost->right)
        {
            temp=rMost;         //temp=下一個rMost的parent
            rMost=rMost->right;
        }
        if(p->key>t->key)       //t為p的left
        {
            p->left=rMost;
            if(rMost->left)
                temp->right=rMost->left;
            rMost->left=t->left;
            rMost->right=t->right;
            delete t;
        }
        else                    //t為p的right
        {
            p->right=rMost;
            if(rMost->left)
                temp->right=rMost->left;
            rMost->left=t->left;
            rMost->right=t->right;
            delete t;
        }

    }
}
//////////////
//END Delete//
//////////////


////////////////////////PolyBST 成員函示/////////////////////////

////////////////////////////////////////////////////////////////
//Operator << (to an ostream) for printing out the polynomial.//
////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& out,PolyBST& b)
{
    static int count;   //用來計算是否為第一個輸出的Node
    count=0;
    b.inorderOut(b.root,out,count); //用來RVL遞迴Node
    out<<endl;
    return out;
}
void PolyBST::inorderOut(BSTNode<int ,double>* bi,ostream& out,int& count)   //RVL的遞迴，可由最大的key輸出到最小(用來支援Operator <<)
{

    if(bi)          //若非NULL
    {
        inorderOut(bi->right,out,count);

        count++;
        if(bi->element>0&&count>1)              //若coefficient > 0且非第一個輸出
        {
            if(bi->key!=0)  //exp!=0
                out<<" + "<<bi->element<<"*x^"<<bi->key;
            else            //exp==0,不需輸出*x^0
                out<<" + "<<bi->element;
        }
        else if(bi->element<0&&count>1)         //若coefficient < 0且非第一個輸出
        {
            if(bi->key!=0)  //exp!=0
                out<<" - "<<(-bi->element)<<"*x^"<<bi->key;
            else            //exp==0
                out<<" - "<<(-bi->element);
        }
        else    //第一個輸出
        {
            if(bi->key!=0)  //exp!=0
                out<<bi->element<<"*x^"<<bi->key;
            else if(bi->key==0&&bi->element!=0) //exp==0
                out<<bi->element;
        }


        inorderOut(bi->left,out,count);

    }
}
///////////////////
//END operator<< //
///////////////////

void PolyBST::makeEmpty(BSTNode<int ,double>* a)
{
    if(a!=NULL)
    {
        if(a->left!=NULL)makeEmpty(a->left);
        if(a->right!=NULL)makeEmpty(a->right);
        delete a;
    }
    a=NULL;
}

///////////////////////////////////////////////////////
//Operator = for assigning one polynomial to another.//
///////////////////////////////////////////////////////
void PolyBST::operator=(const PolyBST& b)            // Operator = for assigning one polynomial to another.
{
    if(root!=NULL)          //若BST非空則清空
    {
        makeEmpty(root);    //makeEmpty(root);
        root=NULL;          //若不將root再NULL一次則無法清除root，why?
    }
    BSTNode<int ,double>* bi=b.root;
    inorderAssignment(bi);
}
void PolyBST::inorderAssignment(BSTNode<int ,double>* bi)    //中序遞迴assignment每個Node(用來支援Operator =)
{
    if(bi)
    {
        if(bi->left!=NULL)inorderAssignment(bi->left);
        Insert(bi->key,bi->element);
        if(bi->right!=NULL)inorderAssignment(bi->right);

    }

}
//////////////////
//END operator= //
//////////////////

/////////////////////////////////////////
//Operator + for adding two polynomials//
/////////////////////////////////////////
PolyBST PolyBST::operator+(const PolyBST& b)
{
    PolyBST c;
    c=*this;
    inorderAdd(c,b.root);
    return c;
}

void PolyBST::inorderAdd(PolyBST& c,BSTNode<int ,double>* bi)   //中序遞迴走訪b，將b的Node加入c中(用來支援Operator +)
{
    if(bi)
    {
        inorderAdd(c,bi->left);
        if(Search(bi->key)!=0)         //若*this的exp和bi的exp相同就相加coef
        {
            BSTNode<int ,double>* ai=Search(bi->key);
            int sum=ai->element+bi->element;
            if(sum) c.Insert(bi->key,sum);   //若相加的coef值不等於零則insert
            else
                c.Delete(ai->key);
        }
        else                                //不相同直接insert
            c.Insert(bi->key,bi->element);
        inorderAdd(c,bi->right);

    }
}
///////////////////
//END Operator + //
///////////////////

///////////////////////////////////////////////////////////
//Operator – for subtracting one polynomial from another.//
///////////////////////////////////////////////////////////

PolyBST PolyBST::operator-(const PolyBST& b)     //Operator – for subtracting one polynomial from another.
{
    PolyBST c;
    c=*this;
    inorderSub(c,b.root);
    return c;
}
void PolyBST::inorderSub(PolyBST& c,BSTNode<int ,double>* bi)   //遞迴中序走訪b，將b的Node加入*this中(用來支援Operator -)
{
    if(bi)
    {
        inorderSub(c,bi->left);
        if(Search(bi->key)!=0)         //若*this的key和bi的key相同就相減element
        {
            BSTNode<int ,double>* ai=Search(bi->key);
            double sum=ai->element-bi->element;
            if(sum) c.Insert(bi->key,sum);   //若相減的element值不等於零則insert進*this
            else
                c.Delete(ai->key);
        }
        else                                //不相同直接insert
        {
            double insertElement=(-bi->element);    //insert的element要改反值
            c.Insert(bi->key,insertElement);
        }
        inorderSub(c,bi->right);

    }
}
//////////////////
//END operator- //
//////////////////

///////////////////////////////////////////////
//Operator * for multiplying two polynomials.//
///////////////////////////////////////////////
PolyBST PolyBST::operator*(const PolyBST& b)
{
    PolyBST c;
    inorderMutiPolyA(c,b.root);
    return c;
}
void PolyBST::inorderMutiPolyA(PolyBST& c, BSTNode<int ,double>* bi)//中序遞迴b，再遞迴a的Node相乘(用來支援Operator *(poly))
{
    if(bi)
    {
        inorderMutiPolyA(c,bi->left);
        inorderMutiPolyB(c,bi,root);
        inorderMutiPolyA(c,bi->right);
    }
}
void PolyBST::inorderMutiPolyB(PolyBST& c, BSTNode<int ,double>* bi,BSTNode<int ,double>* ai)//中序遞迴a(用來支援Operator *(poly))
{
    if(bi&&ai)
    {
        inorderMutiPolyB(c,bi,ai->left);
        if(c.Search(bi->key+ai->key)!=0)         //若*this的key和bi的key相同就相減element
        {
            BSTNode<int ,double>* d=Search(bi->key+ai->key);
            double sum=bi->element*ai->element+d->element;
            if(sum) c.Insert(bi->key,sum);   //若相減的element值不等於零則insert進*this
            else
            {
                int cKey=bi->key+ai->key;
                c.Delete(cKey);
            }

        }
        else
            c.Insert(bi->key+ai->key,bi->element*ai->element);
        inorderMutiPolyB(c,bi,ai->right);
    }
}
//////////////////////////////
//END operator* for two poly//
//////////////////////////////

///////////////////////////////////////////////
//Operator * for multiplying two polynomials.//
///////////////////////////////////////////////

PolyBST PolyBST::operator*(const double& b) //Operator * for multiplying a polynomial and a double scalar.
{
    PolyBST c;
    inorderMutiDouble(c,root,b);
    return c;
}
void PolyBST::inorderMutiDouble(PolyBST& c,BSTNode<int ,double>* ai,const double& b)//(用來支援Operator *(double))
{
    if(ai)
    {
        inorderMutiDouble(c,ai->left,b);
        c.Insert(ai->key,b*ai->element);
        inorderMutiDouble(c,ai->right,b);
    }
}
/////////////////////////////////////////
//END operator* for a poly and a double//
/////////////////////////////////////////


