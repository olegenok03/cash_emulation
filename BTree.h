#ifndef BTREE
#define BTREE

#include <functional>

#include "ArraySequence.h"
#include "Sequence.h"
#include "Pair.h"

template <class TKey, class TElement>
class BTree;

template <class TKey, class TElement>
class BTreeNode
{
public:
    friend class BTree<TKey, TElement>;

private:
    bool leaf;
    Sequence<Pair<TKey, TElement>> *elements;
    Sequence<BTreeNode<TKey, TElement> *> *children;

    BTreeNode()
    {
        leaf = true;
        elements = new ArraySequence<Pair<TKey, TElement>>();
        children = NULL;
    }

    BTreeNode(TKey key, TElement element, bool leaf = true)
    {
        this->leaf = leaf;
        this->elements = new ArraySequence<Pair<TKey, TElement>>(Pair<TKey, TElement>(key, element), 1);
        if (!leaf)
        {
            BTreeNode<TKey, TElement> *empty = NULL;
            this->children = new ArraySequence<BTreeNode<TKey, TElement> *>(empty, 2);
        }
        else
            this->children = NULL;
    }

    BTreeNode(Sequence<Pair<TKey, TElement>> *elements, Sequence<BTreeNode<TKey, TElement> *> *children = NULL, bool leaf = true)
    {
        this->leaf = leaf;
        this->elements = elements;
        this->children = children;
    }

    BTreeNode(BTreeNode<TKey, TElement> &node)
    {
        this->leaf = node.leaf;
        this->elements = node.elements->Copy();
        if (!node.leaf)
            this->children = node.children->Copy();
        else
            this->children = NULL;
    }

    BTreeNode<TKey, TElement> *Split(int index)
    {
        elements->Remove(index);
        if (!leaf)
            return new BTreeNode<TKey, TElement>(elements->Split(index), children->Split(index + 1), false);
        else
        {
            Sequence<Pair<TKey, TElement>> *right_node_elements = elements->Split(index);
            return new BTreeNode<TKey, TElement>(right_node_elements);
        }
    }

    void InsertNonfull(int index, TKey key, TElement element, BTreeNode<TKey, TElement> *node = NULL)
    {
        if (!leaf)
        {
            children->InsertAt(index + 1, node);
        }
        elements->InsertAt(index, Pair<TKey, TElement>(key, element));
    }

    ~BTreeNode()
    {
        delete elements;
        delete children;
    }
};

template <class TKey, class TElement>
class BTree
{
public:
    BTree()
    {
        this->root = NULL;
        this->count = 0;
    }

    BTree(BTree<TKey, TElement> &bTree) : BTree()
    {
        if (!bTree.root)
        {
            return;
        }
        bTree.Traverse(bTree.root, [this](Pair<TKey, TElement> pair)
                       { this->Add(pair.GetFirst(), pair.GetSecond()); });
    }

    static void InitClass(int new_t)
    {
        t = new_t;
    }

    BTree<TKey, TElement> *Copy() const
    {
        return new BTree<TKey, TElement>(*this);
    }

    int GetCount() const
    {
        return count;
    }

    int GetCapacity() const
    {
        return INT_MAX;
    }

    TElement Get(TKey key) const
    {
        BTreeNode<TKey, TElement> *cur_node = root;
        int index = 0;
        do
        {

            Sequence<Pair<TKey, TElement>> *elements = cur_node->elements;

            index = elements->GetFirstIndexIf([key](Pair<TKey, TElement> pair)
                                              {return (pair.GetFirst() >= key); });

            if (index != elements->GetLength() && elements->Get(index).GetFirst() == key)
            {
                return elements->Get(index).GetSecond();
            }
            else
                cur_node = cur_node->children->Get(index);

        } while (1);
    }

    TElement *GetPointer(TKey key) const
    {
        BTreeNode<TKey, TElement> *cur_node = root;
        int index = 0;
        do
        {
            Sequence<Pair<TKey, TElement>> *elements = cur_node->elements;

            index = elements->GetFirstIndexIf([key](Pair<TKey, TElement> pair)
                                              { return (pair.GetFirst() >= key); });
            if (elements->Get(index).GetFirst() == key)
            {
                return elements->GetPointer(index)->GetSecondPointer();
            }
            else
                cur_node = cur_node->children->Get(index);

        } while (1);
    }

    Sequence<TKey> *GetKeys() const
    {
        if (!root)
        {
            return new ArraySequence<TKey>();
        }

        Sequence<TKey> *keys = new ArraySequence<TKey>();

        Traverse(root, [keys](Pair<TKey, TElement> pair)
                 { keys->Append(pair.GetFirst()); });

        return keys;
    }

    Sequence<TElement> *GetElements() const
    {
        if (!root)
        {
            return new ArraySequence<TElement>();
        }

        Sequence<TElement> *elements = new ArraySequence<TElement>();

        Traverse(root, [elements](Pair<TKey, TElement> pair)
                 { elements->Append(pair.GetSecond()); });

        return elements;
    }

    bool ContainsKey(TKey key) const
    {
        if (!root)
            return false;

        BTreeNode<TKey, TElement> *cur_node = root;
        int index = 0;
        while (1)
        {
            Sequence<Pair<TKey, TElement>> *elements = cur_node->elements;

            index = elements->GetFirstIndexIf([key](Pair<TKey, TElement> pair)
                                              { return (pair.GetFirst() >= key); });

            if (cur_node->leaf && (elements->GetLength() == index || elements->Get(index).GetFirst() != key))
            {
                return false;
            }

            if (elements->GetLength() != index && elements->Get(index).GetFirst() == key)
            {
                return true;
            }

            cur_node = cur_node->children->Get(index);
        }
    }

    void Merge(TKey key)
    {
        if (!root)
            return;

        BTreeNode<TKey, TElement> *cur_node = root, *prev_node = NULL;
        int index = -1, prev_index = -1;
        while (1)
        {
            Sequence<Pair<TKey, TElement>> *elements = cur_node->elements;

            prev_index = index;
            index = elements->GetFirstIndexIf([key](Pair<TKey, TElement> pair)
                                              { return (pair.GetFirst() >= key); });

            if (elements->GetLength() == 2 * t - 1)
            {
                if (!prev_node)
                {
                    Pair<TKey, TElement> middle_pair = elements->Get(t - 1);
                    root = new BTreeNode<TKey, TElement>(middle_pair.GetFirst(),
                                                         middle_pair.GetSecond(), false);

                    BTreeNode<TKey, TElement> *left_node = cur_node;
                    BTreeNode<TKey, TElement> *right_node = left_node->Split(t - 1);
                    root->children->Set(0, left_node);
                    root->children->Set(1, right_node);

                    //std::cout << "Middle element: " << middle_pair.GetFirst()  
                    //    << "[" << t - 1 << " of " << 2 * t - 1 << "]" << "\n";
                    

                    if (index > t - 1)
                    {
                        index -= t;
                        cur_node = right_node;
                    }
                    else
                    {
                        cur_node = left_node;
                    }
                }
                else
                {
                    Pair<TKey, TElement> middle_pair = elements->Get(t - 1);

                    BTreeNode<TKey, TElement> *left_node = cur_node;
                    BTreeNode<TKey, TElement> *right_node = left_node->Split(t - 1);

                    prev_node->InsertNonfull(prev_index, middle_pair.GetFirst(), middle_pair.GetSecond(), right_node);

                    if (index > t - 1)
                    {
                        index -= t;
                        cur_node = right_node;
                    }
                }
            }

            if (cur_node->leaf)
            {
                return;
            }

            prev_node = cur_node;
            cur_node = cur_node->children->Get(index);
        }
    }

    void Add(TKey key, TElement element)
    {
        if (!root)
        {
            root = new BTreeNode<TKey, TElement>(key, element);
            count = 1;
            return;
        }

        Merge(key);

        BTreeNode<TKey, TElement> *cur_node = root;
        int index = -1;
        while (1)
        {
            Sequence<Pair<TKey, TElement>> *elements = cur_node->elements;

            //std::cout << "Started debug BNode GetFirstIndexIf, length: " << elements->GetLength() << "\n";

            index = elements->GetFirstIndexIf([key](Pair<TKey, TElement> pair)
                                              {//std::cout << "Элемент: " << pair.GetFirst() << ", ключ: " << key << "\n";
                                                 return (pair.GetFirst() >= key); });

            //std::cout << "Finished debug BNode GetFirstIndexIf, key: " << key 
            //    << ", length: " << elements->GetLength() << ", index: " << index << "\n";

            if (cur_node->leaf)
            {
                //std::cout << "Debug BNode InsertNonfull started\n";
                cur_node->InsertNonfull(index, key, element);
                count++;
                //std::cout << "Debug BNode InsertNonfull finished\n";
                return;
            }

            cur_node = cur_node->children->Get(index);
        }
    }

    void Remove(TKey key) {} // сделать

    BTree<TKey, TElement> &operator=(const BTree<TKey, TElement> &tree)
    {
        BTree<TKey, TElement> temp = *(BTree<TKey, TElement> *)tree.Copy();
        Swap();
        return *this;
    }

    ~BTree()
    {
        if (root)
            TraverseAndDelete(root);
    }

private:
    static int t;

    BTreeNode<TKey, TElement> *root;

    int count;

    void Swap(BTree<TKey, TElement> &tree)
    {
        int bubble_degree = t;

        t = tree.t;

        tree.t = bubble_degree;
    }

    void Traverse(BTreeNode<TKey, TElement> *cur_node, std::function<void(Pair<TKey, TElement>)> func)
    {
        int n = cur_node->elements->GetLength();
        for (int i = 0; i < n; i++)
        {
            func(cur_node->elements->Get(i));
            if (!cur_node->leaf)
                Traverse(cur_node->children->Get(i), func);
        }
        if (!cur_node->leaf)
            Traverse(cur_node->children->GetLast(), func);
    }

    void Traverse(BTreeNode<TKey, TElement> *cur_node, std::function<void(Pair<TKey, TElement>)> func) const
    {
        const_cast<BTree<TKey, TElement> *>(this)->Traverse(cur_node, func);
    }

    void TraverseAndDelete(BTreeNode<TKey, TElement> *cur_node)
    {
        int n = cur_node->elements->GetLength();
        for (int i = 0; i < n + 1; i++)
        {
            if (!cur_node->leaf)
                TraverseAndDelete(cur_node->children->Get(i));
        }
        delete cur_node;
    }
};

template <class TKey, class TElement>
int BTree<TKey, TElement>::t = 0;

#endif