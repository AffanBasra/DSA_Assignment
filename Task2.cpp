#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <random>
using namespace std;

struct Node {
    long long data;
    Node* next;
    Node(long long val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;

    // Helper function for modular multiplication
    LinkedList multiplyMod(const LinkedList& other, const LinkedList& modulus) const {
        LinkedList result;
        result.append(0);  // Initialize with 0
        
        Node* currentThis = head;
        int position = 0;
        
        while (currentThis != nullptr) {
            LinkedList tempResult;
            long long carry = 0;
            
            // Add zeros based on position
            for (int i = 0; i < position; i++) {
                tempResult.append(0);
            }
            
            Node* currentOther = other.head;
            while (currentOther != nullptr) {
                long long product = currentThis->data * currentOther->data + carry;
                tempResult.append(product % 1000000000000000000LL);
                carry = product / 1000000000000000000LL;
                currentOther = currentOther->next;
            }
            
            if (carry > 0) {
                tempResult.append(carry);
            }
            
            result = (result + tempResult) % modulus;
            currentThis = currentThis->next;
            position++;
        }
        
        return result;
    }

public:
    LinkedList() : head(nullptr), size(0) {}
    
       LinkedList(long long value) : head(nullptr), size(0) {
        append(value);
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void append(long long value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    LinkedList operator+(const LinkedList& other) const {
        LinkedList result;
        Node* thisNode = head;
        Node* otherNode = other.head;
        long long carry = 0;

        while (thisNode != nullptr || otherNode != nullptr || carry > 0) {
            long long sum = carry;
            if (thisNode != nullptr) {
                sum += thisNode->data;
                thisNode = thisNode->next;
            }
            if (otherNode != nullptr) {
                sum += otherNode->data;
                otherNode = otherNode->next;
            }

            result.append(sum % 1000000000000000000LL);
            carry = sum / 1000000000000000000LL;
        }

        return result;
    }

    LinkedList operator%(const LinkedList& modulus) const {
        LinkedList result = *this;
        while (result > modulus || result == modulus) {
            result = result - modulus;
        }
        return result;
    }
    bool operator==(const LinkedList& other) const {
        Node* thisNode = head;
        Node* otherNode = other.head;

        while (thisNode != nullptr && otherNode != nullptr) {
            if (thisNode->data != otherNode->data) {
                return false;
            }
            thisNode = thisNode->next;
            otherNode = otherNode->next;
        }

        // Both lists should be exhausted for equality
        return thisNode == nullptr && otherNode == nullptr;
    }
    bool operator>(const LinkedList& other) const {
        vector<long long> thisVec, otherVec;
        Node* thisNode = head;
        Node* otherNode = other.head;

        while (thisNode != nullptr) {
            thisVec.push_back(thisNode->data);
            thisNode = thisNode->next;
        }
        while (otherNode != nullptr) {
            otherVec.push_back(otherNode->data);
            otherNode = otherNode->next;
        }

        if (thisVec.size() != otherVec.size())
            return thisVec.size() > otherVec.size();

        for (int i = thisVec.size() - 1; i >= 0; i--) {
            if (thisVec[i] != otherVec[i])
                return thisVec[i] > otherVec[i];
        }
        return false;
    }

    LinkedList operator-(const LinkedList& other) const {
        LinkedList result;
        Node* thisNode = head;
        Node* otherNode = other.head;
        long long borrow = 0;
        vector<long long> values;

        while (thisNode != nullptr || otherNode != nullptr) {
            long long thisVal = (thisNode != nullptr) ? thisNode->data : 0;
            long long otherVal = (otherNode != nullptr) ? otherNode->data : 0;
            
            if (borrow) {
                if (thisVal > 0) {
                    thisVal--;
                    borrow = 0;
                } else {
                    thisVal = 999999999999999999LL;
                }
            }
            
            if (thisVal < otherVal) {
                thisVal += 1000000000000000000LL;
                borrow = 1;
            }
            
            values.push_back(thisVal - otherVal);
            
            if (thisNode) thisNode = thisNode->next;
            if (otherNode) otherNode = otherNode->next;
        }

        // Remove leading zeros
        while (!values.empty() && values.back() == 0) {
            values.pop_back();
        }

        // Create result LinkedList
        for (int i = 0; i < values.size(); i++) {
            result.append(values[i]);
        }

        if (result.head == nullptr) {
            result.append(0);
        }

        return result;
    }

    bool isEven() const {
        if (head == nullptr) return true;
        return (head->data % 2 == 0);
    }

    LinkedList divideByTwo() const {
        LinkedList result;
        Node* current = head;
        long long carry = 0;
        
        while (current != nullptr) {
            long long value = current->data + carry * 1000000000000000000LL;
            result.append(value / 2);
            carry = value % 2;
            current = current->next;
        }
        
        return result;
    }

    bool isZero() const {
        if (head == nullptr) return true;
        Node* current = head;
        while (current != nullptr) {
            if (current->data != 0) return false;
            current = current->next;
        }
        return true;
    }

    bool isOne() const {
        if (head == nullptr || head->next != nullptr) return false;
        return head->data == 1;
    }

    void findRandD(int& r, LinkedList& d) const {
        r = 0;
        d = *this - LinkedList(1);  // n-1
        
        while (d.isEven()) {
            d = d.divideByTwo();
            r++;
        }
    }

    LinkedList modPow(const LinkedList& exponent, const LinkedList& modulus) const {
        LinkedList result;
        result.append(1);  // Start with 1
        
        LinkedList base = *this % modulus;
        LinkedList exp = exponent;
        
        while (!exp.isZero()) {
            if (!exp.isEven()) {
                result = result.multiplyMod(base, modulus);
            }
            base = base.multiplyMod(base, modulus);
            exp = exp.divideByTwo();
        }
        
        return result;
    }

    static LinkedList generateRandomSmaller(const LinkedList& n) {
        LinkedList result;
        random_device rd;
        mt19937_64 gen(rd());
        
        Node* current = n.head;
        bool significant = false;
        
        while (current != nullptr) {
            uniform_int_distribution<long long> dis(0, current->data);
            long long randomVal = dis(gen);
            
            if (!significant && randomVal < current->data) {
                significant = true;
            }
            
            if (significant) {
                result.append(randomVal);
            }
            
            current = current->next;
        }
        
        if (result.isZero()) {
            result.append(2);  // Ensure we don't return 0 or 1
        }
        
        return result;
    }

    bool isProbablePrime(int k = 4) const {
        if (isEven()) return false;
        
        int r;
        LinkedList d;
        findRandD(r, d);
        
        for (int i = 0; i < k; i++) {
            LinkedList a = generateRandomSmaller(*this);
            LinkedList x = a.modPow(d, *this);
            
            if (x.isOne() || x == *this - LinkedList(1)) continue;
            
            bool witnessFound = false;
            for (int j = 0; j < r - 1; j++) {
                x = x.multiplyMod(x, *this);
                
                if (x.isOne()) return false;
                if (x == *this - LinkedList(1)) {
                    witnessFound = true;
                    break;
                }
            }
            
            if (!witnessFound) return false;
        }
        
        return true;
    }

    void printList() const {
        Node* temp = head;
        bool isFirst = true;
        while (temp != nullptr) {
            if (!isFirst) {
                cout << setfill('0') << setw(18);
            }
            cout << temp->data;
            if (temp->next != nullptr) {
                cout << " -> ";
            }
            temp = temp->next;
            isFirst = false;
        }
        cout << endl;
    }
};

int main() {
    const int CHUNK_SIZE = 18;
    string num_string;
    
    cout << "Enter 1024 bit number to test for primality: ";
    cin >> num_string;

    LinkedList list;

    for (int i = 0; i < num_string.length(); i += CHUNK_SIZE) {
        string sub_string;
        if (i + CHUNK_SIZE <= num_string.length()) {
            sub_string = num_string.substr(i, CHUNK_SIZE);
        } else {
            sub_string = num_string.substr(i);
        }
        long long node_val = stoll(sub_string);
        list.append(node_val);
    }

    cout << "Number to test: ";
    list.printList();

    cout << "Testing primality..." << endl;
    bool isPrime = list.isProbablePrime();
    
    if (isPrime) {
        cout << "The number is probably prime!" << endl;
    } else {
        cout << "The number is definitely composite." << endl;
    }

    return 0;
}