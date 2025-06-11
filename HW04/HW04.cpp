#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Book 
{
public:
	string title;
	string author;

	Book(const string& title, const string& author)
		: title(title), author(author) {}
};

class BorrowManager
{
private:
	struct StockInfo
	{
		int currentQuantity;
		int maxQuantity;
	};

	unordered_map<string, StockInfo> stock;

public:
	void initializeStock(Book* book, int quantity)
	{
		stock[book->title] = { quantity, quantity };
	}

	void borrowBook(const string& title)
	{
		auto it = stock.find(title);

		if (it == stock.end())
		{
			cout << "> Error: �ý��ۿ� ��ϵ��� ���� å�Դϴ�." << endl;
			return;
		}
		
		if (it->second.currentQuantity <= 0)
		{
			cout << "> Notify: '" << title << "' å�� ���� ��� �����ϴ�." << endl;
			return;
		}

		it->second.currentQuantity--;
		cout << "> Success: '" << title << "' å�� �뿩�߽��ϴ�. (���� ���: " << it->second.currentQuantity << ")" << endl;
	}

	void returnBook(const string& title)
	{
		auto it = stock.find(title);

		if (it == stock.end())
		{
			cout << "> Error: �ý��ۿ� ��ϵ��� ���� å�� �ݳ� �õ��߽��ϴ�." << endl;
			return;
		}

		if (it->second.currentQuantity >= it->second.maxQuantity)
		{
			cout << "> Notify: '" << title << "' å�� �̹� ��� ��� �ݳ��� �����Դϴ�." << endl;
			return;
		}

		it->second.currentQuantity++;
		cout << "> Success: '" << title << "' å�� �ݳ��߽��ϴ�. (���� ���: " << it->second.currentQuantity << ")" << endl;
	}

	void displayStock()
	{
		if (stock.empty())
		{
			cout << "> ���� ��ϵ� å�� �����ϴ�." << endl;
			return;
		}

		cout << "> ���� ���� ����� ��� ��:" << endl;
		for (auto i : stock)
		{
			cout << "\t- å: " << i.first << " | ���: " << i.second.currentQuantity << endl;
		}
	}

	// Ư�� å�� �뿩 ���� Ȯ�� �Լ�
	bool isAvailable(const string& title) const
	{
		auto it = stock.find(title);
		
		if (it != stock.end())
		{
			return it->second.currentQuantity > 0;
		}
		return false;
	}
};

class BookManager 
{
private:
	vector<Book> books; // å ��� ����
	BorrowManager borrowManager;

public:
	// å �߰� �޼���
	void addBook(const string& title, const string& author, int quantity = 3) 
	{
		if (findBookByTitle(title) != nullptr)
		{
			cout << "> Error: �̹� ������ ������ å�� �����մϴ�." << endl;
			return;
		}

		books.push_back(Book(title, author)); // push_back ���

		Book* book = findBookByTitle(title);
		borrowManager.initializeStock(book, quantity);
		cout << "> Success: '" << title << "' by " << author << " (�ʱ� ���: " << quantity << ") å�� �߰��Ǿ����ϴ�." << endl;
	}

	void borrowBookByTitle(const string& title)
	{
		borrowManager.borrowBook(title);
	}
	void returnBookByTitle(const string& title)
	{
		borrowManager.returnBook(title);
	}
	void displayStock()
	{
		borrowManager.displayStock();
	}

	// ��� å ��� �޼���
	void displayAllBooks() const 
	{
		if (books.empty()) 
		{
			cout << "> ���� ��ϵ� å�� �����ϴ�." << endl;
			return;
		}

		cout << "> ���� ���� ���:" << endl;
		for (size_t i = 0; i < books.size(); i++) 
		{ 
			// �Ϲ����� for�� ���
			cout << "\t- " << books[i].title << " by " << books[i].author;
			
			if (borrowManager.isAvailable(books[i].title))
			{
				cout << " (�뿩 ����)" << endl;
			}
			else
			{
				cout << " (�뿩 �Ұ�)" << endl;
			}
		}
	}
	
	Book* findBookByTitle(string title);
	Book* findBookByAuthor(string author);
};

Book* BookManager::findBookByTitle(string title)
{
	for (auto& i : books)
	{
		if (i.title == title)
		{
			return &i;
		}
	}

	return nullptr;
}

Book* BookManager::findBookByAuthor(string author)
{
	for (auto& i : books)
	{
		if (i.author == author)
		{
			return &i;
		}
	}

	return nullptr;
}

int main() 
{
	BookManager manager;

	// ������ ���� ���α׷��� �⺻ �޴��� �ݺ������� ����Ͽ� ����� �Է��� ó���մϴ�.
	// ���α׷� ���Ḧ �����ϱ� ������ ��� �����մϴ�.
	while (true) 
	{
		// å1: The Lord of the Rings / J.R.R. Tolkien
		// å2: 1984 / George Orwell
		cout << "\n===== ������ ���� ���α׷� =====" << endl;
		cout << "1. å �߰�" << endl; // å ������ �Է¹޾� å ��Ͽ� �߰�
		cout << "2. å �뿩" << endl;
		cout << "3. å �ݳ�" << endl;
		cout << "4. ��ü å ��� �� ��� ��Ȳ ���" << endl; // ���� å ��Ͽ� �ִ� ��� å ���
		cout << "5. ����" << endl; // ���α׷� ����
		cout << "=============================" << endl;
		cout << "����: ";

		int choice; // ������� �޴� ������ ����
		cin >> choice;

		if (choice == 1) 
		{
			// 1�� ����: å �߰�
			// ����ڷκ��� å ����� ���ڸ��� �Է¹޾� BookManager�� �߰��մϴ�.
			string title, author;
			int quantity;

			cout << "å ����: ";
			cin.ignore(); // ���� �Է��� �ܿ� ���۸� ����
			getline(cin, title); // ���� �Է� (���� ����)
			cout << "å ����: ";
			getline(cin, author); // ���ڸ� �Է� (���� ����)
			cout << "�ʱ� ����: ";
			cin >> quantity;

			manager.addBook(title, author, quantity); // �Է¹��� å ������ �߰�
		}
		else if (choice == 2)
		{
			string title;
			cout << "�뿩�� å ����: ";
			cin.ignore(); // ���� �Է��� �ܿ� ���۸� ����
			getline(cin, title);
			manager.borrowBookByTitle(title);
		}
		else if (choice == 3)
		{
			string title;
			cout << "�ݳ��� å ����: ";
			cin.ignore(); // ���� �Է��� �ܿ� ���۸� ����
			getline(cin, title);
			manager.returnBookByTitle(title);
		}
		else if (choice == 4)
		{
			manager.displayAllBooks();
			manager.displayStock();
		}
		else if (choice == 5) 
		{
			// 3�� ����: ����
			// ���α׷��� �����ϰ� ����ڿ��� �޽����� ����մϴ�.
			cout << "���α׷��� �����մϴ�." << endl;
			break; // while ���� ����
		}
		else 
		{
			// �߸��� �Է� ó��
			// �޴��� ���� ��ȣ�� �Է����� ��� ��� �޽����� ����մϴ�.
			cout << "�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���." << endl;
		}
	}

	return 0; // ���α׷� ���� ����
}
