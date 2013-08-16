#include <iostream>
#include <vector>
#include <set>

class Publication;

class Author {
    std::string name;
    std::string work;
    std::vector<Publication *> publications;

    Author();

    public:
    Author(std::string Name, std::string Work)
        : name(Name)
        , work(Work) {
    }

    friend std::ostream& operator<< (std::ostream& out, const Author&);
    friend bool operator< (const Author&, const Author&);
};



std::ostream& operator<< (std::ostream& out, const Author& author) {
    std::cout << "Author: name = " << author.name << " work = " << author.work;
    return out;
}

bool operator< (const Author& left, const Author& right) {
    return left.name < right.name;
}



class AuthorList {
    static std::set<Author> allAuthors;

    std::vector<std::set<Author>::const_iterator> authors;
    public:
    AuthorList() {
    }

    AuthorList(std::istream& in) {
        std::string name;
        std::string work;

        while (in >> name) {
            if (name == "#")
                break;

            in >> work;

            addAuthor(Author(name, work));
        }
    }

    static void readAuthors(std::istream& in) {
        std::string name;
        std::string work;
        while (in >> name) {
            if (name == "#")
                break;

            in >> work;
            allAuthors.insert(Author(name, work));
        }
    }

    void addAuthor(const Author& author) {
        std::set<Author>::const_iterator it = allAuthors.find(author);

        if (it != allAuthors.end()) {
            authors.push_back(it);
        }
    }

    friend std::ostream& operator<< (std::ostream&, const AuthorList&);
};

std::set<Author> AuthorList::allAuthors;



std::ostream& operator<< (std::ostream& out, const AuthorList& authorList) {
    std::cout << "AUTHORS: " << std::endl;
    for (std::vector<std::set<Author>::const_iterator>::const_iterator
         it = authorList.authors.begin();
         it != authorList.authors.end();
         ++it) {
        std::cout << **it << std::endl;
    }
    return out;
}



class Publication {
    protected:
    std::string name;
    unsigned year;
    AuthorList authors;

    Publication() {
    }

    Publication(std::string Name, unsigned Year)
        : name(Name)
        , year(Year)
        , authors() {
    }


    public:
    void printAuthors() const {
        std::cout << authors << std::endl;
    }

    std::string getName() const {
        return name;
    }

    unsigned getYear() const {
        return year;
    }

    virtual ~Publication() {
    }
};



class Article: public Publication {
    std::string magazine;
    public:
    Article() {
    }

    Article(std::string Magazine, std::string Name, unsigned Year)
        : Publication(Name, Year)
        , magazine(magazine) {
    }

    Article(std::istream& in) {
        in >> magazine >> name >> year;

        authors = AuthorList(in);
    }
};



class Book: public Publication {
    std::string publisher;
    public:
    Book() {
    }

    Book(std::string Publisher, std::string Name, unsigned Year)
        : Publication(Name, Year)
        , publisher(Publisher) {
    }

    Book(std::istream& in) {
        in >> publisher >> name >> year;

        authors = AuthorList(in);
    }
};



class Abstract: public Publication {
    unsigned numberOfThesis;
    public:
    Abstract() {
    }

    Abstract(unsigned NumberOfThesis, std::string Name, unsigned Year)
        : Publication(Name, Year)
        , numberOfThesis(NumberOfThesis) {
    }

    Abstract(std::istream& in) {
        in >> numberOfThesis >> name >> year;

        authors = AuthorList(in);
    }
};



class Internet: public Publication {
    std::string url;
    public:
    Internet() {
    }

    Internet(std::string Url, std::string Name, unsigned Year)
        : Publication(Name, Year)
        , url(Url) {
    }

    Internet(std::istream& in) {
        in >> url >> name >> year;

        authors = AuthorList(in);
    }
};




void readPublications(std::istream& in) {
    std::string type;
    while (in >> type) {
        if (type == "Book") {
            Book b(in);
        } else if (type == "Article") {
            Article a(in);
        } else if (type == "Internet") {
            Internet i(in);
        } else if (type == "Abstract") {
            Abstract a(in);
        } else {
            std::cout << "UNKNOWN TYPE" << std::endl;
        }
    }
}



int main() {
    AuthorList::readAuthors(std::cin);

    readPublications(std::cin);
    return 0;
}
