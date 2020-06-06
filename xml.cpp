#include "xml.h"

bool readXmlFile(std::string filename, std::list<std::string> & commList)
{
    pugi::xml_document doc;

    if(!doc.load_file(filename.data()))
    {
        std::cerr << "Error XML file opening" << std::endl;
        return false;
    }

    std::cout << "XML file loaded" << std::endl;

    pugi::xml_node message = doc.child("message");

    for (pugi::xml_node_iterator it = message.begin(); it != message.end(); ++it)
    {
        if(std::string(it->name()) == "command")
        {
            commList.push_back(it->child_value());
        }
    }

    std::cout << "Commands were successfully loaded. The count is " + std::to_string(commList.size()) << std::endl;
    return true;
}
