#include <iostream>
#include <string>
#include <limits>
#include <utility>
using namespace std;

struct BookRequestNode
{
    int studentID;
    string studentName;
    int bookID;
    string bookTitle;
    int priority;      
    BookRequestNode* next;
};

class LinkedList
{
private:
    BookRequestNode* head;
    int count;

    BookRequestNode* GetNodeAt(int index)
    {
        BookRequestNode* current = head;
        for (int i = 0; i < index && current != nullptr; i++)
            current = current->next;
        return current;
    }

 
    void CopyRequestData(BookRequestNode* a, BookRequestNode* b)
    {
        swap(a->studentID, b->studentID);
        swap(a->studentName, b->studentName);
        swap(a->bookID, b->bookID);
        swap(a->bookTitle, b->bookTitle);
        swap(a->priority, b->priority);
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    // Insert at head.
    void Insert(BookRequestNode* request)
    {
        if (request == nullptr) return;
        request->next = head;
        head = request;
        count++;
    }


    BookRequestNode* Remove()
    {
        if (IsEmpty()) return nullptr;
        BookRequestNode* request = head;
        head = head->next;
        request->next = nullptr;
        count--;
        return request;
    }

    bool IsEmpty() { return head == nullptr; }
    bool IsNotEmpty() { return head != nullptr; }
    int GetCount() { return count; }

   
    void InsertAt(BookRequestNode* request, int index)
    {
        if (request == nullptr) return;
        if (index < 0) index = 0;
        if (index > count) index = count;

        if (index == 0)
        {
            Insert(request);
            return;
        }

        BookRequestNode* previous = GetNodeAt(index - 1);
        request->next = previous->next;
        previous->next = request;
        count++;
    }

  
    BookRequestNode* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        if (index < 0) index = 0;
        if (index >= count) index = count - 1;
        if (index == 0) return Remove();

        BookRequestNode* previous = GetNodeAt(index - 1);
        BookRequestNode* request = previous->next;
        previous->next = request->next;
        request->next = nullptr;
        count--;
        return request;
    }

    void Swap(int index1, int index2)
    {
        if (index1 < 0 || index1 >= count ||
            index2 < 0 || index2 >= count || index1 == index2)
            return;

        CopyRequestData(GetNodeAt(index1), GetNodeAt(index2));
    }


    void Sort()
    {
        for (BookRequestNode* i = head; i != nullptr; i = i->next)
        {
            for (BookRequestNode* j = i->next; j != nullptr; j = j->next)
            {
                if (i->priority > j->priority ||
                    (i->priority == j->priority && i->bookID > j->bookID))
                    CopyRequestData(i, j);
            }
        }
    }

    void Reverse()
    {
        BookRequestNode* previous = nullptr;
        BookRequestNode* current = head;
        while (current != nullptr)
        {
            BookRequestNode* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        head = previous;
    }

    void Print()
    {
        if (IsEmpty())
        {
            cout << "No pending requests.\n";
            return;
        }

        BookRequestNode* current = head;
        int index = 0;
        cout << "\nIndex\tStudent ID\tStudent Name\tBook ID\tPriority\tBook Title\n";
        cout << "--------------------------------------------------------------------------\n";
        while (current != nullptr)
        {
            cout << index << "\t" << current->studentID << "\t\t"
                 << current->studentName << "\t\t" << current->bookID
                 << "\t" << current->priority << "\t\t"
                 << current->bookTitle << '\n';
            current = current->next;
            index++;
        }
        cout << "Total requests: " << count << '\n';
    }

    ~LinkedList()
    {
        while (!IsEmpty())
            delete Remove();
    }
};

BookRequestNode* CreateRequest(int studentID, const string& studentName,
                               int bookID, const string& bookTitle, int priority)
{
    BookRequestNode* request = new BookRequestNode;
    request->studentID = studentID;
    request->studentName = studentName;
    request->bookID = bookID;
    request->bookTitle = bookTitle;
    request->priority = priority;
    request->next = nullptr;
    return request;
}

int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number.\n";
    }
}

BookRequestNode* ReadRequest()
{
    int studentID = ReadInt("Enter Student ID: ");
    string studentName, bookTitle;
    cout << "Enter Student Name: ";
    getline(cin, studentName);
    int bookID = ReadInt("Enter Book ID: ");
    cout << "Enter Book Title: ";
    getline(cin, bookTitle);

    int priority;
    do
    {
        priority = ReadInt("Priority (1=Urgent, 2=Assignment/Project, 3=Normal): ");
        if (priority < 1 || priority > 3)
            cout << "Priority must be 1, 2, or 3.\n";
    } while (priority < 1 || priority > 3);

    return CreateRequest(studentID, studentName, bookID, bookTitle, priority);
}

void ShowMenu()
{
    cout << "\n===== LIBRARY BOOK REQUEST SYSTEM =====\n"
         << "1. Display All Requests\n"
         << "2. Insert Request at Head\n"
         << "3. Remove Request from Head\n"
         << "4. Insert Request at Index\n"
         << "5. Remove Request at Index\n"
         << "6. Swap Requests\n"
         << "7. Sort Requests by Priority\n"
         << "8. Reverse Request List\n"
         << "9. Check Collection Status\n"
         << "10. Exit\n";
}

int main()
{
    LinkedList requests;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            requests.Print();
            break;

        case 2:
            requests.Insert(ReadRequest());
            cout << "Request inserted at head.\n";
            break;

        case 3:
        {
            BookRequestNode* removed = requests.Remove();
            if (removed)
            {
                cout << "Removed request for " << removed->studentName
                     << ", book: " << removed->bookTitle << '\n';
                delete removed;
            }
            else cout << "List is empty.\n";
            break;
        }

        case 4:
        {
            int index = ReadInt("Enter index (zero-based): ");
            requests.InsertAt(ReadRequest(), index);
            cout << "Request inserted.\n";
            break;
        }

        case 5:
        {
            int index = ReadInt("Enter index (zero-based): ");
            BookRequestNode* removed = requests.RemoveAt(index);
            if (removed)
            {
                cout << "Removed request for " << removed->studentName
                     << ", book: " << removed->bookTitle << '\n';
                delete removed;
            }
            else cout << "List is empty.\n";
            break;
        }

        case 6:
        {
            int first = ReadInt("Enter first index: ");
            int second = ReadInt("Enter second index: ");
            if (first < 0 || first >= requests.GetCount() ||
                second < 0 || second >= requests.GetCount())
                cout << "Invalid index. Valid indexes are 0 to "
                     << requests.GetCount() - 1 << ".\n";
            else
            {
                requests.Swap(first, second);
                cout << "Requests swapped.\n";
            }
            break;
        }

        case 7:
            requests.Sort();
            cout << "Requests sorted by priority (then book ID).\n";
            break;

        case 8:
            requests.Reverse();
            cout << "Request list reversed.\n";
            break;

        case 9:
            if (requests.IsEmpty())
                cout << "Status: EMPTY.\n";
            else
                cout << "Status: NOT EMPTY. Pending requests: "
                     << requests.GetCount() << '\n';
            break;

        case 10:
            cout << "Exiting. Remaining nodes will be released.\n";
            break;

        default:
            cout << "Invalid choice. Select 1-10.\n";
        }
    } while (choice != 10);

    return 0;
}