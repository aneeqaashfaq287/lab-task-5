#include <iostream>
#include <string>
#include <limits>
#include <utility>
using namespace std;

struct OrderNode
{
    int orderID;
    string customerName;
    string items;
    string address;
    int priority;     
    OrderNode* next;
};

class LinkedList
{
private:
    OrderNode* head;
    int count;

    OrderNode* GetNodeAt(int index)
    {
        OrderNode* current = head;
        for (int i = 0; i < index && current != nullptr; i++)
            current = current->next;
        return current;
    }

    void CopyOrderData(OrderNode* a, OrderNode* b)
    {
        swap(a->orderID, b->orderID);
        swap(a->customerName, b->customerName);
        swap(a->items, b->items);
        swap(a->address, b->address);
        swap(a->priority, b->priority);
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    void Insert(OrderNode* order)
    {
        if (order == nullptr) return;
        order->next = head;
        head = order;
        count++;
    }

    OrderNode* Remove()
    {
        if (IsEmpty()) return nullptr;
        OrderNode* order = head;
        head = head->next;
        order->next = nullptr;
        count--;
        return order;
    }

    bool IsEmpty() { return head == nullptr; }
    bool IsNotEmpty() { return head != nullptr; }
    int GetCount() { return count; }

    void InsertAt(OrderNode* order, int index)
    {
        if (order == nullptr) return;
        if (index < 0) index = 0;
        if (index > count) index = count;

        if (index == 0)
        {
            Insert(order);
            return;
        }

        OrderNode* previous = GetNodeAt(index - 1);
        order->next = previous->next;
        previous->next = order;
        count++;
    }

    OrderNode* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        if (index < 0) index = 0;
        if (index >= count) index = count - 1;
        if (index == 0) return Remove();

        OrderNode* previous = GetNodeAt(index - 1);
        OrderNode* order = previous->next;
        previous->next = order->next;
        order->next = nullptr;
        count--;
        return order;
    }

    void Swap(int index1, int index2)
    {
        if (index1 < 0 || index1 >= count ||
            index2 < 0 || index2 >= count || index1 == index2)
            return;

        CopyOrderData(GetNodeAt(index1), GetNodeAt(index2));
    }

   
    void Sort()
    {
        for (OrderNode* i = head; i != nullptr; i = i->next)
        {
            for (OrderNode* j = i->next; j != nullptr; j = j->next)
            {
                if (i->orderID > j->orderID)
                    CopyOrderData(i, j);
            }
        }
    }

    void Reverse()
    {
        OrderNode* previous = nullptr;
        OrderNode* current = head;
        while (current != nullptr)
        {
            OrderNode* nextNode = current->next;
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
            cout << "No pending orders.\n";
            return;
        }

        OrderNode* current = head;
        int index = 0;
        cout << "\nIndex\tOrder ID\tCustomer\tPriority\tItems\t\tAddress\n";
        cout << "----------------------------------------------------------------------------\n";
        while (current != nullptr)
        {
            cout << index << "\t" << current->orderID << "\t\t"
                 << current->customerName << "\t\t" << current->priority
                 << "\t\t" << current->items << "\t" << current->address << '\n';
            current = current->next;
            index++;
        }
        cout << "Total orders: " << count << '\n';
    }

    ~LinkedList()
    {
        while (!IsEmpty())
            delete Remove();
    }
};

OrderNode* CreateOrder(int orderID, const string& customerName,
                       const string& items, const string& address, int priority)
{
    OrderNode* order = new OrderNode;
    order->orderID = orderID;
    order->customerName = customerName;
    order->items = items;
    order->address = address;
    order->priority = priority;
    order->next = nullptr;
    return order;
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

OrderNode* ReadOrder()
{
    int orderID = ReadInt("Enter Order ID: ");
    string customerName, items, address;
    cout << "Enter Customer Name: ";
    getline(cin, customerName);
    cout << "Enter Items: ";
    getline(cin, items);
    cout << "Enter Delivery Address: ";
    getline(cin, address);

    int priority;
    do
    {
        priority = ReadInt("Priority (1=Express, 2=Standard, 3=Scheduled): ");
        if (priority < 1 || priority > 3)
            cout << "Priority must be 1, 2, or 3.\n";
    } while (priority < 1 || priority > 3);

    return CreateOrder(orderID, customerName, items, address, priority);
}

string PriorityName(int priority)
{
    if (priority == 1) return "Express Delivery";
    if (priority == 2) return "Standard Delivery";
    return "Scheduled Delivery";
}

void ShowMenu()
{
    cout << "\n===== FOOD DELIVERY ORDER SYSTEM =====\n"
         << "1. Display All Orders\n"
         << "2. Insert Order at Head\n"
         << "3. Remove Order from Head\n"
         << "4. Insert Order at Index\n"
         << "5. Remove Order at Index\n"
         << "6. Swap Orders\n"
         << "7. Sort Orders by Order ID\n"
         << "8. Reverse Order List\n"
         << "9. Check Collection Status\n"
         << "10. Exit\n";
}

int main()
{
    LinkedList orders;

    // Initial sample data from the individual project.
    orders.InsertAt(CreateOrder(1001, "Ali", "Chicken Biryani",
                                "House 12, Model Town", 3), 0);
    orders.InsertAt(CreateOrder(1002, "Sara", "Zinger Burger Meal",
                                "Flat 5, Gulberg", 1), 1);
    orders.InsertAt(CreateOrder(1003, "Bilal", "Pizza Large",
                                "Street 7, DHA", 2), 2);
    orders.InsertAt(CreateOrder(1004, "Hamza", "Pasta and Soup",
                                "House 3, Johar Town", 1), 3);
    orders.InsertAt(CreateOrder(1005, "Ayesha", "BBQ Platter",
                                "Block B, Faisal Town", 2), 4);

    cout << "Five sample food-delivery orders loaded.\n";
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            orders.Print();
            break;

        case 2:
            orders.Insert(ReadOrder());
            cout << "Order inserted at head.\n";
            break;

        case 3:
        {
            OrderNode* removed = orders.Remove();
            if (removed)
            {
                cout << "Removed order " << removed->orderID
                     << " for " << removed->customerName << ".\n";
                delete removed;
            }
            else cout << "List is empty.\n";
            break;
        }

        case 4:
        {
            int index = ReadInt("Enter index (zero-based): ");
            orders.InsertAt(ReadOrder(), index);
            cout << "Order inserted.\n";
            break;
        }

        case 5:
        {
            int index = ReadInt("Enter index (zero-based): ");
            OrderNode* removed = orders.RemoveAt(index);
            if (removed)
            {
                cout << "Removed order " << removed->orderID
                     << " for " << removed->customerName << ".\n";
                delete removed;
            }
            else cout << "List is empty.\n";
            break;
        }

        case 6:
        {
            int first = ReadInt("Enter first index: ");
            int second = ReadInt("Enter second index: ");
            if (first < 0 || first >= orders.GetCount() ||
                second < 0 || second >= orders.GetCount())
                cout << "Invalid index. Valid indexes are 0 to "
                     << orders.GetCount() - 1 << ".\n";
            else
            {
                orders.Swap(first, second);
                cout << "Orders swapped.\n";
            }
            break;
        }

        case 7:
            orders.Sort();
            cout << "Orders sorted by Order ID in ascending order.\n";
            break;

        case 8:
            orders.Reverse();
            cout << "Order list reversed.\n";
            break;

        case 9:
            if (orders.IsEmpty())
                cout << "Status: EMPTY.\n";
            else
                cout << "Status: NOT EMPTY. Pending orders: "
                     << orders.GetCount() << '\n';
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