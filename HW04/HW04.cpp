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
			cout << "> Error: 시스템에 등록되지 않은 책입니다." << endl;
			return;
		}
		
		if (it->second.currentQuantity <= 0)
		{
			cout << "> Notify: '" << title << "' 책은 현재 재고가 없습니다." << endl;
			return;
		}

		it->second.currentQuantity--;
		cout << "> Success: '" << title << "' 책을 대여했습니다. (남은 재고: " << it->second.currentQuantity << ")" << endl;
	}

	void returnBook(const string& title)
	{
		auto it = stock.find(title);

		if (it == stock.end())
		{
			cout << "> Error: 시스템에 등록되지 않은 책을 반납 시도했습니다." << endl;
			return;
		}

		if (it->second.currentQuantity >= it->second.maxQuantity)
		{
			cout << "> Notify: '" << title << "' 책은 이미 모든 재고가 반납된 상태입니다." << endl;
			return;
		}

		it->second.currentQuantity++;
		cout << "> Success: '" << title << "' 책을 반납했습니다. (현재 재고: " << it->second.currentQuantity << ")" << endl;
	}

	void displayStock()
	{
		if (stock.empty())
		{
			cout << "> 현재 등록된 책이 없습니다." << endl;
			return;
		}

		cout << "> 현재 도서 목록의 재고 수:" << endl;
		for (auto i : stock)
		{
			cout << "\t- 책: " << i.first << " | 재고: " << i.second.currentQuantity << endl;
		}
	}

	// 특정 책의 대여 여부 확인 함수
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
	vector<Book> books; // 책 목록 저장
	BorrowManager borrowManager;

public:
	// 책 추가 메서드
	void addBook(const string& title, const string& author, int quantity = 3) 
	{
		if (findBookByTitle(title) != nullptr)
		{
			cout << "> Error: 이미 동일한 제목의 책이 존재합니다." << endl;
			return;
		}

		books.push_back(Book(title, author)); // push_back 사용

		Book* book = findBookByTitle(title);
		borrowManager.initializeStock(book, quantity);
		cout << "> Success: '" << title << "' by " << author << " (초기 재고: " << quantity << ") 책이 추가되었습니다." << endl;
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

	// 모든 책 출력 메서드
	void displayAllBooks() const 
	{
		if (books.empty()) 
		{
			cout << "> 현재 등록된 책이 없습니다." << endl;
			return;
		}

		cout << "> 현재 도서 목록:" << endl;
		for (size_t i = 0; i < books.size(); i++) 
		{ 
			// 일반적인 for문 사용
			cout << "\t- " << books[i].title << " by " << books[i].author;
			
			if (borrowManager.isAvailable(books[i].title))
			{
				cout << " (대여 가능)" << endl;
			}
			else
			{
				cout << " (대여 불가)" << endl;
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

	// 도서관 관리 프로그램의 기본 메뉴를 반복적으로 출력하여 사용자 입력을 처리합니다.
	// 프로그램 종료를 선택하기 전까지 계속 동작합니다.
	while (true) 
	{
		// 책1: The Lord of the Rings / J.R.R. Tolkien
		// 책2: 1984 / George Orwell
		cout << "\n===== 도서관 관리 프로그램 =====" << endl;
		cout << "1. 책 추가" << endl; // 책 정보를 입력받아 책 목록에 추가
		cout << "2. 책 대여" << endl;
		cout << "3. 책 반납" << endl;
		cout << "4. 전체 책 목록 및 재고 현황 출력" << endl; // 현재 책 목록에 있는 모든 책 출력
		cout << "5. 종료" << endl; // 프로그램 종료
		cout << "=============================" << endl;
		cout << "선택: ";

		int choice; // 사용자의 메뉴 선택을 저장
		cin >> choice;

		if (choice == 1) 
		{
			// 1번 선택: 책 추가
			// 사용자로부터 책 제목과 저자명을 입력받아 BookManager에 추가합니다.
			string title, author;
			int quantity;

			cout << "책 제목: ";
			cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
			getline(cin, title); // 제목 입력 (공백 포함)
			cout << "책 저자: ";
			getline(cin, author); // 저자명 입력 (공백 포함)
			cout << "초기 수량: ";
			cin >> quantity;

			manager.addBook(title, author, quantity); // 입력받은 책 정보를 추가
		}
		else if (choice == 2)
		{
			string title;
			cout << "대여할 책 제목: ";
			cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
			getline(cin, title);
			manager.borrowBookByTitle(title);
		}
		else if (choice == 3)
		{
			string title;
			cout << "반납할 책 제목: ";
			cin.ignore(); // 이전 입력의 잔여 버퍼를 제거
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
			// 3번 선택: 종료
			// 프로그램을 종료하고 사용자에게 메시지를 출력합니다.
			cout << "프로그램을 종료합니다." << endl;
			break; // while 루프 종료
		}
		else 
		{
			// 잘못된 입력 처리
			// 메뉴에 없는 번호를 입력했을 경우 경고 메시지를 출력합니다.
			cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
		}
	}

	return 0; // 프로그램 정상 종료
}
