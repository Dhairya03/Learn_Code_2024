class Book 
{
    public function getTitle() {
        return "A Great Book";
    }

    public function getAuthor() {
        return "John Doe";
    }

    public function getCurrentPage() {
        return "current page content";
    }

    public function turnPage() {
        // pointer to next page
    }
}

class Library
{
    public function getBookLocation(Book $book) {
        // returns the position in the library
        // ie. shelf number & room number
    }
}

class BookSaver
{
    public function saveBook(Book $book) {
        $filename = '/documents/'. $book->getTitle(). ' - ' . $book->getAuthor();
        file_put_contents($filename, serialize($book));
    }
}

interface Printer {
    public function printPage($page);
}

class PlainTextPrinter implements Printer {
    public function printPage($page) {
        echo $page;
    }
}

class HtmlPrinter implements Printer {
    public function printPage($page) {
        echo '<div style="single-page">' . $page . '</div>';
    }
}