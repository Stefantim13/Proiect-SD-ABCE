#include <bits/stdc++.h>
using namespace std;

ifstream f("abce.in");
ofstream g("abce.out");

class BTreeNode
{
    long long *keys;
    long long t;
    BTreeNode **C;
    long long n;
    bool leaf;

public:
    BTreeNode(long long _t, bool _leaf);
    void traverse_xy(long long x, long long y);
    BTreeNode *search(long long k);
    BTreeNode *search_succ(long long &succ, long long k);
    BTreeNode *search_pred(long long &pred, long long k);
    long long findKey(long long k);
    void insertNonFull(long long k);
    void splitChild(long long i, BTreeNode *y);
    void remove(long long k);
    void removeFromLeaf(long long idx);
    void removeFromNonLeaf(long long idx);
    long long getPred(long long idx);
    long long getSucc(long long idx);
    void fill(long long idx);
    void borrowFromPrev(long long idx);
    void borrowFromNext(long long idx);
    void merge(long long idx);
    friend class BTree;
};

class BTree
{
    BTreeNode *root;
    long long t;

public:
    BTree(long long _t)
    {
        root = NULL;
        t = _t;
    }

    void traverse_xy(long long x, long long y)
    {
        if (root != NULL)
            root->traverse_xy(x, y);
    }

    BTreeNode *search(long long k)
    {
        if (root == NULL)
        {
            g << "0\n";
            return NULL;
        }
        else
            return root->search(k);
    }

    BTreeNode *search_succ(long long k)
    {
        long long succ = 1e10;
        root->search_succ(succ, k);
        g << succ << "\n";
        return NULL;
    }

    BTreeNode *search_pred(long long k)
    {
        long long pred = -1e10;
        root->search_pred(pred, k);
        g << pred << "\n";
        return NULL;
    }

    void insert(long long k);
    void remove(long long k);
};

BTreeNode::BTreeNode(long long t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;
    keys = new long long[2 * t - 1];
    C = new BTreeNode *[2 * t];
    n = 0;
}

long long BTreeNode::findKey(long long k)
{
    long long idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;
    return idx;
}

void BTreeNode::remove(long long k)
{
    long long idx = findKey(k);
    if (idx < n && keys[idx] == k)
    {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {
        if (leaf)
        {
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (C[idx]->n < t)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
    return;
}

void BTreeNode::removeFromLeaf(long long idx)
{
    for (long long i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    n--;
    return;
}

void BTreeNode::removeFromNonLeaf(long long idx)
{
    long long k = keys[idx];
    if (C[idx]->n >= t)
    {
        long long pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    }
    else if (C[idx + 1]->n >= t)
    {
        long long succ = getSucc(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    }
    else
    {
        merge(idx);
        C[idx]->remove(k);
    }
    return;
}

long long BTreeNode::getPred(long long idx)
{
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

long long BTreeNode::getSucc(long long idx)
{
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void BTreeNode::fill(long long idx)
{
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

void BTreeNode::borrowFromPrev(long long idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];
    for (long long i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf)
    {
        for (long long i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }
    child->keys[0] = keys[idx - 1];
    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];
    keys[idx - 1] = sibling->keys[sibling->n - 1];
    child->n += 1;
    sibling->n -= 1;
    return;
}

void BTreeNode::borrowFromNext(long long idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];
    child->keys[(child->n)] = keys[idx];
    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];
    keys[idx] = sibling->keys[0];
    for (long long i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf)
    {
        for (long long i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }
    child->n += 1;
    sibling->n -= 1;
    return;
}

void BTreeNode::merge(long long idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];
    child->keys[t - 1] = keys[idx];
    for (long long i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];
    if (!child->leaf)
    {
        for (long long i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }
    for (long long i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    for (long long i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];
    child->n += sibling->n + 1;
    n--;
    delete (sibling);
    return;
}

void BTree::insert(long long k)
{
    if (root == NULL)
    {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else
    {
        if (root->n == 2 * t - 1)
        {
            BTreeNode *s = new BTreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);
            long long i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);
            root = s;
        }
        else
            root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(long long k)
{
    long long i = n - 1;
    if (leaf == true)
    {
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n = n + 1;
    }
    else
    {
        while (i >= 0 && keys[i] > k)
            i--;
        if (C[i + 1]->n == 2 * t - 1)
        {
            splitChild(i + 1, C[i + 1]);
            if (keys[i + 1] < k)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(long long i, BTreeNode *y)
{
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
    for (long long j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];
    if (y->leaf == false)
    {
        for (long long j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }
    y->n = t - 1;
    for (long long j = n; j >= i + 1; j--)
        C[j + 1] = C[j];
    C[i + 1] = z;
    for (long long j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];
    n = n + 1;
}

void BTreeNode::traverse_xy(long long x, long long y)
{
    long long i = 0;
    for (i = 0; i < n; i++)
    {
        if (!leaf)
        {
            C[i]->traverse_xy(x, y);
        }
        if (x <= keys[i] && keys[i] <= y)
        {
            g << keys[i] << " ";
        }
    }
    if (!leaf)
    {
        C[i]->traverse_xy(x, y);
    }
}

BTreeNode *BTreeNode::search(long long k)
{
    long long i = 0;
    while (i < n && k > keys[i])
        i++;
    if (keys[i] == k)
    {
        g << "1\n";
        return this;
    }
    if (leaf == true)
    {
        g << "0\n";
        return NULL;
    }
    return C[i]->search(k);
}

BTreeNode *BTreeNode::search_succ(long long &succ, long long k)
{
    long long i = 0;
    while (i < n && k > keys[i])
        i++;
    if (keys[i] < succ && keys[i] >= k)
    {
        succ = keys[i];
    }
    if (leaf == true)
    {
        return NULL;
    }
    return C[i]->search_succ(succ, k);
}

BTreeNode *BTreeNode::search_pred(long long &pred, long long k)
{
    long long i = n - 1;
    while (i >= 0 && k <= keys[i])
        i--;
    if (keys[i + 1] == k && i < n)
    {
        pred = k;
        return NULL;
    }
    else if (keys[i] > pred && keys[i] <= k && i >= 0)
    {
        pred = keys[i];
    }
    if (leaf == true)
    {
        return NULL;
    }
    return C[i + 1]->search_pred(pred, k);
}

void BTree::remove(long long k)
{
    if (!root)
        return;
    root->remove(k);
    if (root->n == 0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];
        delete tmp;
    }
    return;
}

int main()
{
    short int t;
    long long q, x, y;
    f >> q;

    BTree bt(200);

    for (long long i = 0; i < q; ++i)
    {
        f >> t;
        switch (t)
        {
        case 1:
            f >> x;
            bt.insert(x);
            break;
        case 2:
            f >> x;
            bt.remove(x);
            break;
        case 3:
            f >> x;
            bt.search(x);
            break;
        case 4:
            f >> x;
            bt.search_pred(x);
            break;
        case 5:
            f >> x;
            bt.search_succ(x);
            break;

        case 6:
            f >> x >> y;
            bt.traverse_xy(x, y);
            g << "\n";
            break;
        default:
            g << "wrong input\n";
        }
    }
}