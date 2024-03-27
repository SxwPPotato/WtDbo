#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Windows.h>
#include <set>


#pragma execution_character_set( "utf-8" )

class book;

class publisher {
public:
	
	std::string name = "";	
	Wt::Dbo::collection<Wt::Dbo::ptr<book> > id_publisher;
	template<typename Action>
	void persist(Action& a) 
	{
		Wt::Dbo::hasMany(a, id_publisher, Wt::Dbo::ManyToOne, "publisher");
		Wt::Dbo::field(a, name, "name");
		
	}

};

class stock;

class book {
public:
	
	std::string title = "";
	Wt::Dbo::ptr<publisher>  id_publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<stock> > id_book;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::hasMany(a, id_book, Wt::Dbo::ManyToOne, "book");
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, id_publisher, "publisher");
	
	}
};

class shop;
class sale;

class stock {
public:
	Wt::Dbo::collection<Wt::Dbo::ptr<sale> > id_stock;
	Wt::Dbo::ptr<book>  id_book;
	Wt::Dbo::ptr<shop>  id_shop;
	int count;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::hasMany(a, id_stock, Wt::Dbo::ManyToOne, "stock");
		Wt::Dbo::belongsTo(a, id_book, "book");
		Wt::Dbo::belongsTo(a, id_shop, "shop");
		Wt::Dbo::field(a, count, "count");
	}

};

class shop {
public:
	Wt::Dbo::collection<Wt::Dbo::ptr<stock> > id_shop;
	std::string name = "";

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::hasMany(a, id_shop, Wt::Dbo::ManyToOne, "shop");
		Wt::Dbo::field(a, name, "name");
	}

};

class sale {
public:
	int price;
	std::string date_sale ;
	Wt::Dbo::ptr<stock>  id_stock;
	int count;

	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, id_stock, "stock");
		Wt::Dbo::field(a, count, "count");
	}

};

int main()
{

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	try {
		std::string connectionstring = "host=localhost "
			"port=5432 "
			"dbname=shop "
			"user=postgres "
			"password=0406Dbnfkz2005";

		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connectionstring);

		Wt::Dbo::Session s;		

		s.setConnection(std::move(connection));
		s.mapClass<publisher>("publisher");
		s.mapClass<book>("book");
		s.mapClass<stock>("stock");
		s.mapClass<shop>("shop");
		s.mapClass<sale>("sale");
		
		s.dropTables();
		s.createTables();

		Wt::Dbo::Transaction t(s);

		std::unique_ptr<publisher> publisher1(new publisher{ "Harper Collins"  });

		auto pub1 = s.add(std::move(publisher1));

		std::unique_ptr<publisher> publisher2(new publisher{ "Random House" });

		auto pub2 = s.add(std::move(publisher2));

		std::unique_ptr<publisher> publisher3(new publisher{ "Macmillian Publishers" });

		auto pub3 = s.add(std::move(publisher3));


		//Книги

		std::unique_ptr<book> book1(new book{ "Harry Potter and the Philosopher's Stone ",pub1 });

		std::unique_ptr<book> book2(new book{ "The Lord of the Rings", pub1 });

		std::unique_ptr<book> book3(new book{ "Harry Potter and the Chamber of Secrets", pub2 });

		std::unique_ptr<book> book4(new book{ "Harry Potter and the Prisoner of Azkaban", pub2 });

		std::unique_ptr<book> book5(new book{ "Harry Potter and the Goblet of Fire", pub2 });

		std::unique_ptr<book> book6(new book{ "Harry Potter and the Order of the Phoenix", pub3 });

		std::unique_ptr<book> book7(new book{ "Harry Potter and the Half-Blood Prince", pub3 });

		auto bk1 = s.add(std::move(book1));
		auto bk2 = s.add(std::move(book2));
		auto bk3 = s.add(std::move(book3));
		auto bk4 = s.add(std::move(book4));
		auto bk5 = s.add(std::move(book5));
		auto bk6 = s.add(std::move(book6));
		auto bk7 = s.add(std::move(book7));

		//shop

		std::unique_ptr<shop> shop1(new shop{ {}, "Bart's Books" });

		auto sh1 = s.add(std::move(shop1));

		std::unique_ptr<shop>  shop2(new shop{ {}, "The Last Bookstore" });

		auto sh2 = s.add(std::move(shop2));

		std::unique_ptr<shop>  shop3(new shop{ {}, "Hatchards" });

		auto sh3 = s.add(std::move(shop3));

		//stock

		std::unique_ptr<stock> stock1(new stock{ {}, bk1, sh1, 20 });
		std::unique_ptr<stock> stock2(new stock{ {}, bk2, sh1, 10 });
		std::unique_ptr<stock> stock3(new stock{ {}, bk6, sh1, 15 });
		std::unique_ptr<stock> stock4(new stock{ {}, bk1, sh2, 35 });
		std::unique_ptr<stock> stock5(new stock{ {}, bk3, sh2, 45 });
		std::unique_ptr<stock> stock6(new stock{ {}, bk7, sh2, 30 });
		std::unique_ptr<stock> stock7(new stock{ {}, bk2, sh2, 20 });
		std::unique_ptr<stock> stock8(new stock{ {}, bk4, sh3, 25 });
		std::unique_ptr<stock> stock9(new stock{ {}, bk5, sh3, 40 });

		auto stk1 = s.add(std::move(stock1));
		auto stk2 = s.add(std::move(stock2));
		auto stk3 = s.add(std::move(stock3));
		auto stk4 = s.add(std::move(stock4));
		auto stk5 = s.add(std::move(stock5));
		auto stk6 = s.add(std::move(stock6));
		auto stk7 = s.add(std::move(stock7));
		auto stk8 = s.add(std::move(stock8));
		auto stk9 = s.add(std::move(stock9));

		//sale

		std::unique_ptr<sale> sale1(new sale{ 30,"2024-28-03", stk1, 20 });
		std::unique_ptr<sale> sale2(new sale{ 25,"2024-21-05", stk5, 30 });
		std::unique_ptr<sale> sale3(new sale{ 20,"2024-03-04", stk6, 20 });
		std::unique_ptr<sale> sale4(new sale{ 35,"2024-31-03", stk8, 15 });

		s.add(std::move(sale1));
		s.add(std::move(sale2));
		s.add(std::move(sale3));
		s.add(std::move(sale4));
		t.commit();


		{
			Wt::Dbo::Transaction t2(s);

			std::string NameOrId;
			int id;
			bool IdOrNot = false;

			std::cout << "Введите имя или идентификатор: ";
			std::getline(std::cin, NameOrId);

			try {
				id = stoi(NameOrId);
				IdOrNot = true;
			}
			catch (...)
			{
				IdOrNot = false;
			}
			Wt::Dbo::ptr<publisher> pb;

			if (IdOrNot) {
				pb = s.find<publisher>().where("id=?").bind(NameOrId);
			}
			else {
				pb = s.find<publisher>().where("name=?").bind(NameOrId);
			}

			if (pb) {
				std::set<std::string> shop_names;

				for (const auto& book : pb->id_publisher) 
				{
					for (const auto stock : book->id_book)
					{
						shop_names.insert(stock->id_shop->name);
						
					}
					
				}

				std::cout << "Name of shop: ";

				for (const auto& shop_name : shop_names) {
					std::cout << shop_name << std::endl;

				}

			}
			else {
				std::cout << "Publisher not found: "  << std::endl;
			}

		}
		

	}
	catch (const std::exception& e)
	{
		
		std::cout << "Eror hapened:" << e.what() << std::endl;
	}

	return 0;
}
