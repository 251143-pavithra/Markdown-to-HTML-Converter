#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

// ==========================================
// 1. HTML OUTPUT CLASS
// Demonstrates encapsulation and operator
// overloading
// ==========================================

class HTMLDocument {
private:
    string content;

public:
    HTMLDocument() {
        content = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<meta charset=\"UTF-8\">\n"
                  "<title>Markdown Website</title>\n"
                  "</head>\n<body>\n";
    }

    void append(const string& text) {
        content += text;
    }

    string getContent() const {
        return content + "\n</body>\n</html>\n";
    }

    // Operator overloading
    HTMLDocument& operator<<(const string& text) {
        content += text;
        return *this;
    }
};


// ==========================================
// 2. TEMPLATE CLASS
// Demonstrates templates
// ==========================================

template <typename T>
class FileUtility {
public:
    static T readFile(const string& filename) {
        T data;

        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error(
                "Cannot open input file: " + filename
            );
        }

        string line;

        while (getline(file, line)) {
            data += line;
            data += '\n';
        }

        return data;
    }

    static void writeFile(
        const string& filename,
        const T& data
    ) {
        ofstream file(filename);

        if (!file.is_open()) {
            throw runtime_error(
                "Cannot create output file: " + filename
            );
        }

        file << data;

        if (!file) {
            throw runtime_error(
                "Error writing output file: " + filename
            );
        }
    }
};


// ==========================================
// 3. BASE PARSER CLASS
// Demonstrates abstraction, inheritance
// and virtual functions
// ==========================================

class MarkdownParser {
public:
    virtual string parse(const string& line) const = 0;

    virtual ~MarkdownParser() = default;
};


// ==========================================
// 4. HEADING PARSER
// Demonstrates inheritance and overriding
// ==========================================

class HeadingParser : public MarkdownParser {
public:
    string parse(const string& line) const override {
        int level = 0;

        while (
            level < static_cast<int>(line.size()) &&
            level < 6 &&
            line[level] == '#'
        ) {
            level++;
        }

        if (
            level > 0 &&
            level < static_cast<int>(line.size()) &&
            line[level] == ' '
        ) {
            string text = line.substr(level + 1);

            return "<h" + to_string(level) + ">" +
                   text +
                   "</h" + to_string(level) + ">";
        }

        return "";
    }
};


// ==========================================
// 5. INLINE PARSER
// Handles bold, italic, code and links
// ==========================================

class InlineParser : public MarkdownParser {
private:
    string replaceAll(
        string text,
        const string& from,
        const string& to
    ) const {
        if (from.empty()) {
            return text;
        }

        size_t position = 0;

        while (
            (position = text.find(from, position))
            != string::npos
        ) {
            text.replace(position, from.length(), to);
            position += to.length();
        }

        return text;
    }

    string convertLinks(string text) const {
        size_t pos = 0;

        while ((pos = text.find('[', pos)) != string::npos) {
            size_t middle = text.find("](", pos);
            size_t end = text.find(')', middle);

            if (
                middle == string::npos ||
                end == string::npos
            ) {
                break;
            }

            string label = text.substr(
                pos + 1,
                middle - pos - 1
            );

            string url = text.substr(
                middle + 2,
                end - middle - 2
            );

            string html =
                "<a href=\"" + url + "\">" +
                label + "</a>";

            text.replace(
                pos,
                end - pos + 1,
                html
            );

            pos += html.length();
        }

        return text;
    }

public:
    string parse(const string& line) const override {
        string result = line;

        // Inline code
        result = replaceAll(result, "`", "");

        // Bold
        result = replaceAll(result, "**", "<b>");
        result = replaceAll(result, "<b>", "<b>");

        // Italic
        result = replaceAll(result, "*", "<i>");

        // Links
        result = convertLinks(result);

        return result;
    }
};


// ==========================================
// 6. MAIN MARKDOWN CONVERTER
// Demonstrates polymorphism, memory
// management, string processing and a
// simple state machine
// ==========================================

class MarkdownConverter {
private:
    vector<unique_ptr<MarkdownParser>> parsers;

    enum class State {
        NORMAL,
        CODE_BLOCK,
        LIST
    };

    static bool startsWith(
        const string& text,
        const string& prefix
    ) {
        return text.rfind(prefix, 0) == 0;
    }

public:
    MarkdownConverter() {
        parsers.push_back(
            make_unique<HeadingParser>()
        );

        parsers.push_back(
            make_unique<InlineParser>()
        );
    }

    string convert(const string& markdown) const {
        HTMLDocument html;

        istringstream input(markdown);

        string line;

        State state = State::NORMAL;

        bool listOpen = false;

        while (getline(input, line)) {

            // CODE BLOCK STATE
            if (startsWith(line, "```")) {
                if (state == State::CODE_BLOCK) {
                    html << "</code></pre>\n";
                    state = State::NORMAL;
                } else {
                    if (listOpen) {
                        html << "</ul>\n";
                        listOpen = false;
                    }

                    html << "<pre><code>";
                    state = State::CODE_BLOCK;
                }

                continue;
            }

            // Preserve code block content
            if (state == State::CODE_BLOCK) {
                html << line << "\n";
                continue;
            }

            // Blank line
            if (line.empty()) {
                if (listOpen) {
                    html << "</ul>\n";
                    listOpen = false;
                }

                continue;
            }

            // UNORDERED LIST
            if (startsWith(line, "- ")) {
                if (!listOpen) {
                    html << "<ul>\n";
                    listOpen = true;
                }

                string item = line.substr(2);

                for (const auto& parser : parsers) {
                    item = parser->parse(item);
                }

                html << "<li>" + item + "</li>\n";

                continue;
            }

            if (listOpen) {
                html << "</ul>\n";
                listOpen = false;
            }

            // POLYMORPHIC PARSING
            string result;

            bool isHeading = false;

            for (const auto& parser : parsers) {
                string parsed = parser->parse(line);

                if (!parsed.empty()) {
                    result = parsed;

                    if (startsWith(line, "#")) {
                        isHeading = true;
                    }
                }
            }

            if (isHeading) {
                html << result + "\n";
            } else {
                html << "<p>" + result + "</p>\n";
            }
        }

        // Close any open structures
        if (state == State::CODE_BLOCK) {
            html << "</code></pre>\n";
        }

        if (listOpen) {
            html << "</ul>\n";
        }

        return html.getContent();
    }
};


// ==========================================
// 7. MAIN FUNCTION
// Demonstrates exception handling
// ==========================================

int main() {
    try {
        cout << "================================\n";
        cout << "  MARKDOWN TO HTML CONVERTER\n";
        cout << "================================\n";

        string inputFile;
        string outputFile;

        cout << "Enter Markdown input filename: ";
        cin >> inputFile;

        cout << "Enter HTML output filename: ";
        cin >> outputFile;

        // Read Markdown file
        string markdown =
            FileUtility<string>::readFile(inputFile);

        // Create converter object
        MarkdownConverter converter;

        // Convert Markdown to HTML
        string html = converter.convert(markdown);

        // Save generated HTML
        FileUtility<string>::writeFile(
            outputFile,
            html
        );

        cout << "\nConversion successful!\n";
        cout << "HTML saved to: " << outputFile << endl;

    } catch (const exception& e) {
        cerr << "\nError: " << e.what() << endl;
        return 1;
    }

    return 0;
}
