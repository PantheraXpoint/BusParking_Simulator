#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;


class Interval {
public:
    int start;
    int end;
    
    Interval(int start = 0, int end = 0) {
        this->start = start;
        this->end = end;
    }
};

int minPark(vector<Interval> lInterval) {
    if (!lInterval.size())
        return 0;
    int max = 0;
    map<int, int> rank;
    int* ret = new int[lInterval.size() * 2];
    vector<int> A; int init = 0;
    for (unsigned int i = 0; i < lInterval.size(); i++)
    {
        ret[i] = 0;
        A.push_back(lInterval[init].start);
        A.push_back(lInterval[init].end);
        init++;
    }
    for (unsigned int i = lInterval.size(); i < lInterval.size() * 2; i++)
        ret[i] = 0;
    sort(A.begin(), A.end());
    vector<int>::iterator it;
    it = unique(A.begin(), A.end());
    A.resize(distance(A.begin(), it));
    for (unsigned int i = 0; i < A.size(); i++)
    {
        map<int, int>::iterator ite;
        ite = rank.begin();
        rank.insert(ite, pair<int, int>(A[i], i));
    }
    for (unsigned int i = 0; i < lInterval.size(); i++)
    {
        map<int, int>::iterator it1;
        map<int, int>::iterator it2;
        it1 = rank.find(lInterval[i].start);
        it2 = rank.find(lInterval[i].end);
        ret[(*it1).second] += 1;
        ret[(*it2).second] -= 1;
    }
    max = ret[0];
    for (unsigned int i = 1; i < lInterval.size() * 2; i++)
    {
        int temp = ret[i - 1];
        ret[i] += temp;
        if (ret[i] > max) max = ret[i];
    }
    return max;
}

class BusParking
{
public:
    class DynamicIT {
    public:
        const int left = 1, right = 1e9;

        struct Node {
            Node* l, * r;
            int fmax, cnt;

            Node() {
                l = r = NULL;
                fmax = cnt = 0;
            }
        } *root = new Node();

        int getFmax(Node* x) {
            return x == NULL ? 0 : x->fmax;
        }

        int getCnt(Node* x) {
            return x == NULL ? 0 : x->cnt;
        }

        void updateFmax(Node* x) {
            x->fmax = max(getFmax(x->l), getFmax(x->r)) + x->cnt;
        }

        void update(Node* k, int l, int r, int L, int R, int val) { // val = 1 -> insert , val = -1 -> remove
            if (L > r || l > R) return;
            if (L <= l && r <= R) {
                k->cnt += val;
                updateFmax(k);
                return;
            }
            if (k->l == NULL) k->l = new Node();
            if (k->r == NULL) k->r = new Node();
            int mid = (l + r) >> 1;
            update(k->l, l, mid, L, R, val);
            update(k->r, mid + 1, r, L, R, val);
            updateFmax(k);
        }

        void newQuery(int l, int r, int val) {
            update(root, left, right, l, r, val);
        }
    };

    class Start_Node
    {
    public:
        int start;
        map<int,int>End_Nodes;
    public:
        Start_Node() {}
        Start_Node(int s, int e)
        {
            this->start = s;
            if (this->End_Nodes.empty())
            {
                this->End_Nodes.insert(pair<int,int>(e,0));
            }
            else
            {
                if (this->End_Nodes.find(e) != this->End_Nodes.end())
                {
                    this->End_Nodes.insert(pair<int, int>(e, 0));
                }
            }
        }
        ~Start_Node() {}
    };
public:
    map<int,Start_Node>input;
    DynamicIT tree;

public:
    BusParking(){}
    ~BusParking(){}
	void add(int s, int t) {
        if (input.find(s) == input.end())
        {
            map<int, Start_Node>::iterator it; it = input.begin();
            Start_Node N(s, t);
            input.insert(it, pair<int, Start_Node>(s,N));
        }
        else
        {
            map<int, Start_Node>::iterator it; it = input.find(s);
            if ((*it).second.End_Nodes.find(t) == (*it).second.End_Nodes.end())
                (*it).second.End_Nodes.insert(pair<int,int>(t,0));
            else
                return;
        }
        --t;
        tree.newQuery(s, t, 1);
	}

	void remove(int s, int t) {
        if (input.find(s) != input.end())
        {
            map<int, Start_Node>::iterator it; it = input.find(s);
            if ((*it).second.End_Nodes.find(t) != (*it).second.End_Nodes.end())
                (*it).second.End_Nodes.erase(t);
            else return;
        }
        else
            return;
        --t;
        tree.newQuery(s, t, -1);
	}

	int minPark() {
		return tree.root->fmax;
	}
};

int main()
{
    vector<Interval> intervals;
    intervals.push_back(Interval(1, 5));
    intervals.push_back(Interval(2, 5));
    intervals.push_back(Interval(3, 5));

    cout << minPark(intervals) << endl      ;


    BusParking *b = new BusParking;
    b->add(1, 5);
    b->add(2, 5);
    b->add(3, 5);
    cout << b->minPark();
}