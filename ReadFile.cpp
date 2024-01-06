#include "ReadFile.h"

bool checkScale = false;

bool isParsingGroup = false;

vector<bool> checkRGB;

string chuanhoa(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ',')
        {
            input[i] = ' ';
        }
    }
    istringstream iss(input);
    ostringstream oss;
    string word;
    bool first = true;

    while (iss >> word)
    {

        if (!first)
        {
            oss << " ";
        }
        oss << word;
        first = false;
    }
    return oss.str();
}

void getNextNumberOfValues(string& s, int& i, int num, Path& path, char curType)
{
    int count = 0;
    for (int j = 0; j < num; ++j)
    {
        if (isdigit(s[i]) || s[i] == '-' || s[i] == '.' || s[i] == ' ')
        {
            size_t next_pos;
            double num_ = stod(s.substr(i), &next_pos);
            path.value.push_back(num_);
            i += next_pos;
        }
    }
    path.type.push_back(curType);
}

void convertPathToValue(string s, Path& path)
{
    char currentCommand = '\0';
    path.type.clear();
    path.value.clear();

    bool passM = false;
    s = chuanhoa(s);
    int i = 0;
    while (i < s.length())
    {
        if (isalpha(s[i]))
        {

            currentCommand = s[i];
            if (currentCommand == 'M' || currentCommand == 'm' && !passM)
                passM = true;
            i++;

            switch (currentCommand)
            {
            case 'M':
            case 'm':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'L':
            case 'l':
            case 'T':
            case 't':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'H':
            case 'h':
            case 'V':
            case 'v':
                getNextNumberOfValues(s, i, 1, path, currentCommand);
                break;
            case 'C':
            case 'c':
                getNextNumberOfValues(s, i, 6, path, currentCommand);
                break;
            case 'Z':
            case 'z':
                getNextNumberOfValues(s, i, 0, path, currentCommand);
                break;
            case 'S':
            case 's':
            case 'Q':
            case 'q':
                getNextNumberOfValues(s, i, 4, path, currentCommand);
                break;
            case 'A':
            case 'a':
                getNextNumberOfValues(s, i, 7, path, currentCommand);
                break;
            }
        }
        else
        {
            if (currentCommand == 'm' && passM)
                currentCommand = 'l';
            else if (currentCommand == 'M' && passM)
                currentCommand = 'L';
            i++;
            switch (currentCommand)
            {
            case 'M':
            case 'm':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'L':
            case 'l':
            case 'T':
            case 't':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'H':
            case 'h':
            case 'V':
            case 'v':
                getNextNumberOfValues(s, i, 1, path, currentCommand);
                break;
            case 'C':
            case 'c':
                getNextNumberOfValues(s, i, 6, path, currentCommand);
                break;
            case 'Z':
            case 'z':
                getNextNumberOfValues(s, i, 0, path, currentCommand);
                break;
            case 'S':
            case 's':
            case 'Q':
            case 'q':
                getNextNumberOfValues(s, i, 4, path, currentCommand);
                break;
            case 'A':
            case 'a':
                getNextNumberOfValues(s, i, 7, path, currentCommand);
                break;
            }
        }
        if (s[i] == ' ')
            i++;
        if (s[i] >= '0' && s[i] <= '9' || s[i] == '-' || s[i] == '.')
        {
            i--;
        }
    }
}

string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void convert_letters_to_RGB(RGB& rgb, string s)
{
    map<string, RGB> colorMap =
    {
        {"black", {0, 0, 0}},
        {"darkslategray", {47, 79, 79}},
        {"white", {255, 255, 255}},
        {"none", {255, 255, 255}},
        {"red", {255, 0, 0}},
        {"green", {0, 255, 0}},
        {"grey", {128, 128, 128}},
        {"blue", {0, 0, 255}},
        {"yellow", {255, 255, 0}},
        {"cyan", {0, 255, 255}},
        {"magenta", {255, 0, 255}},
        {"grey", {128, 128, 128}},
        {"lightgrey", {192, 192, 192}},
        {"darkgrey", {64, 64, 64}},
        {"maroon", {128, 0, 0}},
        {"olive", {128, 128, 0}},
        {"green", {138, 43, 226}},
        {"navy", {0, 0, 128}},
        {"purple", {128, 0, 128}},
        {"teal", {0, 128, 128}},
        {"orange", {255, 165, 0}},
        {"brown", {165, 42, 42}},
        {"pink", {255, 182, 193}},
        {"salmon", {250, 128, 114}},
        {"gold", {255, 215, 0}},
        {"lime", {0, 255, 0}},
        {"skyblue", {135, 206, 235}},
        {"forestgreen", {34, 139, 34}},
        {"turquoise", {64, 224, 208}},
        {"steelblue", {70, 130, 180}},
        {"dodgerblue", {30, 144, 255}},
        {"royalblue", {65, 105, 225}},
        {"indigo", {75, 0, 130}},
        {"slateblue", {106, 90, 205}},
        {"lavender", {230, 230, 250}},
        {"thistle", {216, 191, 216}},
        {"plum", {221, 160, 221}},
        {"violet", {238, 130, 238}},
        {"orchid", {218, 112, 214}},
        {"mediumpurple", {147, 112, 219}},
        {"darkorchid", {153, 50, 204}},
        {"darkviolet", {148, 0, 211}},
        {"darkmagenta", {139, 0, 139}},
        {"mediumorchid", {186, 85, 211}},
        {"palevioletred", {219, 112, 147}},
        {"cornflowerblue", {100, 149, 237}},
        {"deepskyblue", {0, 191, 255}},
        {"lightskyblue", {135, 206, 250}},
        {"midnightblue", {25, 25, 112}},
        {"mediumaquamarine", {102, 205, 170}},
        {"darkseagreen", {143, 188, 143}},
        {"lightgreen", {144, 238, 144}},
        {"palegreen", {152, 251, 152}},
        {"springgreen", {0, 255, 127}},
        {"mediumspringgreen", {0, 250, 154}},
        {"darkolivegreen", {85, 107, 47}},
        {"olivedrab", {107, 142, 35}},
        {"chartreuse", {127, 255, 0}},
        {"lawngreen", {124, 252, 0}},
        {"darkgreen", {0, 100, 0}},
        {"mediumseagreen", {60, 179, 113}},
        {"seagreen", {46, 139, 87}},
        {"darkslategray", {47, 79, 79}},
        {"dimgray", {105, 105, 105}},
        {"rosybrown", {188, 143, 143}},
        {"indianred", {205, 92, 92}},
        {"blueviolet", {138, 43, 226}},
        {"skyblue", {135, 206, 235}}

    };

    string lowerCaseS = toLower(s);

    auto it = colorMap.find(lowerCaseS);
    if (it != colorMap.end())
    {
        rgb = it->second;
    }
    else
    {
        rgb = { 255, 255, 255 };
    }
}

string convert_String_to_RGB(RGB& rgb, string s, smatch match, regex re)
{
    rgb = { 0, 0, 0 };
    if ((s[0] == 'r' && s[1] == 'g' && s[2] == 'b') && regex_search(s, match, re))
    {
        if (match.size() == 4)
        {
            rgb.r = stoi(match[1]);
            if (rgb.r > 255)
                rgb.r = 255;
            rgb.g = stoi(match[2]);
            if (rgb.g > 255)
                rgb.g = 255;
            rgb.b = stoi(match[3]);
            if (rgb.b > 255)
                rgb.b = 255;
        }
    }
    else if (s != "")
    {
        if (s[0] != '#')
            convert_letters_to_RGB(rgb, s);

        else if (s[0] == '#')
        {
            unsigned int hexValue;
            if (s.length() == 4)
            {
                char a1 = s[1], a2 = s[2], a3 = s[3];
                s = "#" + string(1, a1) + string(1, a1) + string(1, a2) + string(1, a2) + string(1, a3) + string(1, a3);
            }
            istringstream(s.substr(1)) >> hex >> hexValue;

            rgb.r = (hexValue >> 16) & 0xFF;
            rgb.g = (hexValue >> 8) & 0xFF;
            rgb.b = hexValue & 0xFF;
            int alpha = (hexValue >> 24) & 0xFF;
        }
    }
    return "";
}

void convert_String_to_RGB_(RGB& rgb, string s, smatch match, regex re)
{
    rgb = { 0, 0, 0 };
    if ((s[0] == 'r' && s[1] == 'g' && s[2] == 'b') && regex_search(s, match, re))
    {
        if (match.size() == 4)
        {
            rgb.r = stoi(match[1]);
            if (rgb.r > 255)
                rgb.r = 255;
            rgb.g = stoi(match[2]);
            if (rgb.g > 255)
                rgb.g = 255;
            rgb.b = stoi(match[3]);
            if (rgb.b > 255)
                rgb.b = 255;
        }
    }
    else if (s != "")
    {
        if (s[0] != '#')
            convert_letters_to_RGB(rgb, s);

        else if (s[0] == '#')
        {
            unsigned int hexValue;
            if (s.length() == 4)
            {
                char a1 = s[1], a2 = s[2], a3 = s[3];
                s = "#" + string(1, a1) + string(1, a1) + string(1, a2) + string(1, a2) + string(1, a3) + string(1, a3);
            }
            istringstream(s.substr(1)) >> hex >> hexValue;

            rgb.r = (hexValue >> 16) & 0xFF;
            rgb.g = (hexValue >> 8) & 0xFF;
            rgb.b = hexValue & 0xFF;
            int alpha = (hexValue >> 24) & 0xFF;
        }
    }
}

void convertStyle(string styleString, string& fill, string& stroke, RGB& fillRGB, RGB& strokeRGB, float& fillOpacity, float& strokeOpacity, float& strokeWidth) {
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    int posFill = styleString.find("fill:");
    int posStroke = styleString.find("stroke:");
    int posFillOpa = styleString.find("fill-opacity:");
    int posStrokeOpa = styleString.find("stroke-opacity:");
    int posStrokeWidth = styleString.find("stroke-width:");
    smatch matches;
    if (posFill != string::npos) {
        size_t fillEnd = styleString.find(";", posFill);
        fill = styleString.substr(posFill + 5, fillEnd - posFill - 5);
        if (fill.find("url") == string::npos) {

            fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            int hashtagPos = fill.find("#");
            int stopPos = fill.find(")");
            fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
        }
    }
    if (posStroke != string::npos) {
        size_t strokeEnd = styleString.find(";", posStroke);
        stroke = styleString.substr(posStroke + 7, strokeEnd - posStroke - 7);

        if (stroke.find("url") == string::npos) {

            stroke = convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            int hashtagPos = stroke.find("#");
            int stopPos = stroke.find(")");
            stroke = stroke.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
        }
    }
    else
    {
        strokeOpacity = strokeWidth = 0;
        strokeRGB = { 255,255,255 };
    }
    if (posFillOpa != string::npos) {
        size_t fillOpaEnd = styleString.find(";", posFillOpa);
        string fillOpaValue = styleString.substr(posFillOpa + 13, fillOpaEnd - posFillOpa - 13);
        fillOpacity = stod(fillOpaValue);
    }
    if (posStrokeOpa != string::npos) {
        size_t strokeOpaEnd = styleString.find(";", posStrokeOpa);
        string strokeOpaValue = styleString.substr(posStrokeOpa + 15, strokeOpaEnd - posStrokeOpa - 15);
        strokeOpacity = stod(strokeOpaValue);
    }
    if (posStrokeWidth != string::npos) {
        size_t strokeWidthEnd = styleString.find(";", posStrokeWidth);
        string strokeWidthValue = styleString.substr(posStrokeWidth + 13, strokeWidthEnd - posStrokeWidth - 13);
        strokeWidth = stod(strokeWidthValue);
    }
}

void convertStyleChild(string styleString, string& fill, string& stroke, RGB& fillRGB, RGB& strokeRGB, float& fillOpacity, float& strokeOpacity, float& strokeWidth, groupChild groupChild) {
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    int posFill = styleString.find("fill:");
    int posStroke = styleString.find("stroke:");
    int posFillOpa = styleString.find("fill-opacity:");
    int posStrokeOpa = styleString.find("stroke-opacity:");
    int posStrokeWidth = styleString.find("stroke-width:");
    smatch matches;
    if (posFill != string::npos) {
        size_t fillEnd = styleString.find(";", posFill);
        fill = styleString.substr(posFill + 5, fillEnd - posFill - 5);
        if (fill.find("url") == string::npos) {

            fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            int hashtagPos = fill.find("#");
            int stopPos = fill.find(")");
            fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
        }
    }
    else {
        fillRGB = groupChild.fillRGB;
    }
    if (posStroke != string::npos) {
        size_t strokeEnd = styleString.find(";", posStroke);
        stroke = styleString.substr(posStroke + 7, strokeEnd - posStroke - 7);
        if (stroke.find("url") == string::npos) {

            stroke = convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            int hashtagPos = stroke.find("#");
            int stopPos = stroke.find(")");
            stroke = stroke.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
        }
    }
    else {
        strokeRGB = groupChild.strokeRGB;
    }
    if (posFillOpa != string::npos) {
        size_t fillOpaEnd = styleString.find(";", posFillOpa);
        string fillOpaValue = styleString.substr(posFillOpa + 13, fillOpaEnd - posFillOpa - 13);
        fillOpacity = stod(fillOpaValue);
    }
    else {
        fillOpacity = groupChild.fillOpacity;
    }
    if (posStrokeOpa != string::npos) {
        size_t strokeOpaEnd = styleString.find(";", posStrokeOpa);
        string strokeOpaValue = styleString.substr(posStrokeOpa + 15, strokeOpaEnd - posStrokeOpa - 15);
        strokeOpacity = stod(strokeOpaValue);
    }
    else {
        strokeOpacity = groupChild.strokeOpacity;
    }
    if (posStrokeWidth != string::npos) {
        size_t strokeWidthEnd = styleString.find(";", posStrokeWidth);
        string strokeWidthValue = styleString.substr(posStrokeWidth + 13, strokeWidthEnd - posStrokeWidth - 13);
        strokeWidth = stod(strokeWidthValue);
    }
    else {
        strokeWidth = groupChild.strokeWidth;
    }
}

void convertStyleGradient(string styleString, string& id, string& gradientUnits, string& spreadMethod) {
    int posId = styleString.find("id:");
    int posGradientUnits = styleString.find("gradientUnits:");
    int posSpreadMethod = styleString.find("spreadMethod:");
    if (posId != string::npos) {
        size_t idEnd = styleString.find(";", posId);
        id = styleString.substr(posId + 3, idEnd - posId - 3);
    }
    if (posGradientUnits != string::npos) {
        size_t gradientUnitsEnd = styleString.find(";", posGradientUnits);
        gradientUnits = styleString.substr(posGradientUnits + 14, gradientUnitsEnd - posGradientUnits - 14);
    }
    if (posSpreadMethod != string::npos) {
        size_t spreadMethodEnd = styleString.find(";", posSpreadMethod);
        spreadMethod = styleString.substr(posSpreadMethod + 13, spreadMethodEnd - posSpreadMethod - 13);
    }
}

vector<string> split(string& s, char deli)
{
    vector<string> tokens;
    istringstream tokenStream(s);
    string token;
    while (getline(tokenStream, token, deli))
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool containsKeyword(string& s, string keyword)
{
    return s.find(keyword) != string::npos;
}

string standardize(string transform)
{
    string result;

    int pos = 0;
    while (pos < transform.length())
    {
        int open = transform.find("(", pos);
        if (open == string::npos)
            break;

        int close = transform.find(")", open);
        if (close == string::npos)
            break;

        string content = transform.substr(open + 1, close - open - 1);

        if (content.find(",") == string::npos)
        {
            int space = content.find(" ");
            if (space != string::npos)
            {
                content.insert(space, ",");
            }
        }

        result += transform.substr(pos, open - pos + 1);
        result += content;
        result += ")";

        pos = close + 1;
    }

    result += transform.substr(pos);

    return result;
}

GraphicsState Shape::TransformSVG(Graphics& graphics, Transform transform)
{
    GraphicsState state = graphics.Save();
    Matrix transformMatrix;

    for (const string& operation : transform.transformOrder)
    {
        if (operation == "scale")
            if (checkScale)
                transformMatrix.Scale(transform.scaleX, transform.scaleX);
            else
                transformMatrix.Scale(transform.scaleX, transform.scaleY);
        else if (operation == "translate")
            transformMatrix.Translate(transform.translateX, transform.translateY);
        else if (operation == "rotate")
            transformMatrix.Rotate(transform.rotateAngle);
        else if (operation == "skew")
        {
            float skewX = tan(transform.skewX * static_cast<float>(M_PI) / 180.0f);
            float skewY = tan(transform.skewY * static_cast<float>(M_PI) / 180.0f);

            Matrix skewMatrix(1.0f, skewY, skewX, 1.0f, 0.0f, 0.0f);
            transformMatrix.Multiply(&skewMatrix);
        }
    }
    graphics.MultiplyTransform(&transformMatrix);
    return state;
}

void parseTransform(const string& transformStr, Transform& transform)
{
    transform.transformOrder.clear();
    string transformString = transformStr;
    transformString = standardize(transformString);
    vector<string> transformDeli = split(transformString, ' ');
    for (string& deli : transformDeli)
    {
        if (containsKeyword(deli, "rotate"))
        {
            transform.transformOrder.push_back("rotate");
        }
        else if (containsKeyword(deli, "scale"))
        {
            transform.transformOrder.push_back("scale");
        }
        else if (containsKeyword(deli, "translate"))
        {
            transform.transformOrder.push_back("translate");
        }
    }

    // X? lý translate
    smatch translateMatches;
    regex translateRegex("translate\\(([^,]+),([^)]+)\\)");
    if (regex_search(transformString, translateMatches, translateRegex))
    {
        transform.translateX = stof(translateMatches[1]);
        transform.translateY = stof(translateMatches[2]);
    }
    // X? lý rotate
    regex rotateRegex("rotate\\(([^)]+)\\)");
    smatch rotateMatches;
    if (regex_search(transformString, rotateMatches, rotateRegex))
    {
        transform.rotateAngle = stof(rotateMatches[1]);
    }
    // X? lý scale
    bool check = false;
    regex scaleCheck("scale\\((.*?)\\)");
    smatch scaleCheckMatches;
    regex scaleRegex("scale\\(([^,]+)(?:,([^)]+))?\\)");
    smatch scaleMatches;

    if (regex_search(transformString, scaleCheckMatches, scaleCheck))
    {
        string scalePart = scaleCheckMatches[1];
        if (scalePart.find(",") != string::npos)
            check = true;
    }

    if (regex_search(transformString, scaleMatches, scaleRegex))
    {

        float num1 = stof(scaleMatches[1].str());
        if (check)
        {
            float num2 = stof(scaleMatches[2].str());
            transform.scaleX = num1;
            transform.scaleY = num2;
        }
        else
        {
            checkScale = true;
            transform.scaleY = num1;
            transform.scaleX = num1;
        }
    }
}

void parseTransformMatrix(const string& transformStr, Transform& transform) {
    regex matrixRegex("matrix\\((.*?)\\)");
    smatch matrixMatches;
    if (regex_search(transformStr, matrixMatches, matrixRegex)) {
        string matrixStr = matrixMatches[1].str();
        double skewX, skewY;
        istringstream iss(matrixStr);
        iss >> transform.scaleX >> transform.skewX >> transform.skewY >> transform.scaleY >> transform.translateX >> transform.translateY;
        if (transform.scaleX != 1.0 && transform.scaleY != 1.0)
            transform.transformOrder.push_back("scale");
        if (transform.skewX != 0.0 && transform.skewY != 0.0)
            transform.transformOrder.push_back("skew");
        if (transform.translateX != 0.0 && transform.translateY != 0.0)
            transform.transformOrder.push_back("translate");
    }
    
}

vector<string> mergeVector(vector<string> v1, vector<string> v2)
{
    vector<string> v3;

    for (string s : v1)
    {
        bool found = false;
        for (string x : v2)
        {
            if (s == x)
            {
                found = true;

                break;
            }
        }

        if (!found)
        {
            v3.push_back(s);
        }
    }

    for (string s : v2)
    {
        v3.push_back(s);
    }
    return v3;
}

void parseTransformChild(const string& transformStr, Transform& transform, groupChild& groupChild)
{
    if (transformStr.find("matrix") != string::npos) {
        parseTransformMatrix(transformStr, transform);
    }
    else {
        parseTransform(transformStr, transform);

        if (!transformStr.empty())
        {
            transform.translateX += groupChild.transform.translateX;
            transform.translateY += groupChild.transform.translateY;
            transform.rotateAngle += groupChild.transform.rotateAngle;
            transform.rotateAngle = (transform.rotateAngle > 360) ? (transform.rotateAngle - 360) : transform.rotateAngle;
            transform.scaleX *= groupChild.transform.scaleX;
            transform.scaleY *= groupChild.transform.scaleY;
            transform.transformOrder = mergeVector(groupChild.transform.transformOrder, transform.transformOrder);
        }
        else
        {
            transform = groupChild.transform;
        }
    }

}

void parseTransformChildforText(const string& transformStr, Transform& transform, groupChild& groupChild, Transform& trans)
{
    if (transformStr.find("matrix") != string::npos) {
        parseTransformMatrix(transformStr, transform);
    }
    else {
        parseTransform(transformStr, transform);

        if (!transformStr.empty())
        {
            trans.translateX = transform.translateX;
            trans.translateY = transform.translateY;
            trans.rotateAngle = transform.rotateAngle;
            trans.scaleX = transform.scaleX;
            trans.scaleY = transform.scaleY;

            transform.translateX += groupChild.transform.translateX;
            transform.translateY += groupChild.transform.translateY;
            transform.rotateAngle += groupChild.transform.rotateAngle;
            transform.rotateAngle = (transform.rotateAngle > 360) ? (transform.rotateAngle - 360) : transform.rotateAngle;
            transform.scaleX *= groupChild.transform.scaleX;
            transform.scaleY *= groupChild.transform.scaleY;
            transform.transformOrder = mergeVector(groupChild.transform.transformOrder, transform.transformOrder);
        }
        else
        {
            transform = groupChild.transform;
        }
    }

}

void parseSVGNode(pugi::xml_node& node, vector<Shape*>& elements, groupChild groupChild)
{

    string nodeName = node.name();
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");

    if (nodeName == "circle")
    {
        float cx = node.attribute("cx").as_float();
        float cy = node.attribute("cy").as_float();
        float r = node.attribute("r").as_float();
        smatch matches;
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fill = "";
                fillOpacity = 0;
                fillRGB = { 255, 255, 255 };
            }
            else if (!fill.empty())
            {
                if (fill.find("url") == string::npos) {
                    fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                }
                else if (!fill.empty())
                {
                    int hashtagPos = fill.find("#");
                    int stopPos = fill.find(")");
                    fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                }
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }
            else if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
            }

            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Circle* circle = new Circle(cx, cy, r, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, fill, "");
        elements.push_back(circle);
    }

    else if (nodeName == "rect")
    {

        float x = node.attribute("x").as_float();
        float y = node.attribute("y").as_float();
        float width = node.attribute("width").as_float();
        float height = node.attribute("height").as_float();

        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;

        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fill = "";
                fillOpacity = 0;
                fillRGB = { 255, 255, 255 };
            }
            else if (!fill.empty()) {
                if (fill.find("url") == string::npos) {
                    fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                }
                else if (!fill.empty()) {
                    int hashtagPos = fill.find("#");
                    int stopPos = fill.find(")");
                    fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                }
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }
            else if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
            }

            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Rect_* rect = new Rect_(x, y, width, height, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, fill, "");
        elements.push_back(rect);
    }

    else if (nodeName == "line")
    {
        float x1 = node.attribute("x1").as_float();
        float y1 = node.attribute("y1").as_float();
        float x2 = node.attribute("x2").as_float();
        float y2 = node.attribute("y2").as_float();
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;

        string styleString = node.attribute("style").value();
        if (!styleString.empty())
        {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }
            else if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
            }

            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Line* line = new Line(x1, y1, x2, y2, strokeOpacity, strokeRGB, strokeWidth, transform, "");
        elements.push_back(line);
    }

    else if (nodeName == "text")
    {

        float x = node.attribute("x").as_float();
        float y = node.attribute("y").as_float();
        float dx = node.attribute("dx").as_float();
        float dy = node.attribute("dy").as_float();
        string textAnchor = node.attribute("text-anchor").empty()
            ? "start"
            : node.attribute("text-anchor").value();
        string fontStyle = node.attribute("font-style").empty()
            ? "normal"
            : node.attribute("font-style").value();

        // Content extraction
        string content;
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        {
            if (child.type() == pugi::node_pcdata)
            {
                content = child.value();
                break;
            }
        }

        float fontSize = node.attribute("font-size").as_float();
        if (fontSize == 0)
        {
            fontSize = groupChild.fontSize;
        }
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;
        bool checkk = true;
        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fill = "";
                fillOpacity = 0;
                fillRGB = { 255, 255, 255 };
            }
            else if (!fill.empty()) {
                if (fill.find("url") == string::npos) {
                    fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                }
                else if (!fill.empty()) {
                    int hashtagPos = fill.find("#");
                    int stopPos = fill.find(")");
                    fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                }
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            checkk = true;
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }

            else if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
                if (stroke == "" && strokeRGB.r == 255 && strokeRGB.g == 255 && strokeRGB.b == 255)
                {
                    checkk = 0;
                }                       
            }

            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        Transform trans = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChildforText(transformValue, transform, groupChild, trans);

        // fontFamily
        string fontFamily = node.attribute("font-family").empty()
            ? "Times New Roman"
            : node.attribute("font-family").value();
        Text* text = new Text(x, y, content, fontSize, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fontFamily, dx, dy, textAnchor, fontStyle, checkk, fill, "", trans);
        elements.push_back(text);
    }

    //2 cai poly ko xu li gradient
    else if (nodeName == "polyline")
    {
        string points = node.attribute("points").value();
        bool chekk = true;
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;

        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fillOpacity = 0;
                fillRGB = { 256, 256, 256 };
            }
            else if (!fill.empty())
            {
                convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (!stroke.empty() && stroke != "none")
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
                auto temp = strokeOpacity;
                auto temp_ = strokeWidth;
                for (const auto& checkk : checkRGB)
                {
                    if (checkk)
                    {
                        strokeWidth = temp_;
                        strokeOpacity = temp;
                        break;
                    }
                    strokeWidth = strokeOpacity = 0;
                }
            }
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Polyline_* polyline = new Polyline_(points, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, "", "");
        elements.push_back(polyline);
    }

    else if (nodeName == "polygon")
    {
        string points = node.attribute("points").value();

        bool chekk = true;
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;

        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else
        {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (!fill.empty())
            {
                convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
                auto temp = strokeOpacity;
                auto temp_ = strokeWidth;
                for (const auto& checkk : checkRGB)
                {
                    if (checkk)
                    {
                        strokeWidth = temp_;
                        strokeOpacity = temp;
                        break;
                    }
                    strokeWidth = strokeOpacity = 0;
                }
            }
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Polygon_* polygon = new Polygon_(points, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, "", "");
        elements.push_back(polygon);
    }

    else if (nodeName == "ellipse")
    {
        float cx = node.attribute("cx").as_float();
        float cy = node.attribute("cy").as_float();
        float rx = node.attribute("rx").as_float();
        float ry = node.attribute("ry").as_float();
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;
        bool checkk = true;
        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fill = "";
                fillOpacity = 0;
                fillRGB = { 255, 255, 255 };
            }
            else if (!fill.empty())
            {
                if (fill.find("url") == string::npos)
                {
                    fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                }
                else if (!fill.empty()) {
                    int hashtagPos = fill.find("#");
                    int stopPos = fill.find(")");
                    fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                }
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }
            else if (!stroke.empty())
            {
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
            }

            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Ellipse_* ellipse = new Ellipse_(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fill, "");
        elements.push_back(ellipse);
    }

    else if (nodeName == "path")
    {
        float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
        string fill, stroke;
        RGB fillRGB = { 0, 0, 0 };
        RGB strokeRGB = { 0, 0, 0 };
        smatch matches;
        
        string styleString = node.attribute("style").value();
        if (!styleString.empty()) {
            convertStyleChild(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, groupChild);
        }
        else {
            // FILL
            fillOpacity = node.attribute("fill-opacity").empty()
                ? groupChild.fillOpacity
                : node.attribute("fill-opacity").as_float();
            fill = node.attribute("fill").value();
            if (fill == "none")
            {
                fill = "";
                fillOpacity = 0;
                fillRGB = { 255, 255, 255 };
            }
            else if (!fill.empty())
            {
                if (fill.find("url") == string::npos)
                {
                    fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                }
                else if (!fill.empty()) {
                    int hashtagPos = fill.find("#");
                    int stopPos = fill.find(")");
                    fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                }
            }
            else
            {
                fillRGB = groupChild.fillRGB;
            }

            // STROKE
            strokeWidth = node.attribute("stroke-width").empty()
                ? groupChild.strokeWidth
                : node.attribute("stroke-width").as_float();
            strokeOpacity = node.attribute("stroke-opacity").empty()
                ? groupChild.strokeOpacity
                : node.attribute("stroke-opacity").as_float();
            stroke = node.attribute("stroke").value();
            if (stroke == "none")
            {
                strokeOpacity = 0;
                strokeRGB = { 255, 255, 255 };
            }
            else if (!stroke.empty())
            {
                stroke = convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
            }
            else
            {
                strokeRGB = groupChild.strokeRGB;
                if (groupChild.strokeRGB.r == groupChild.strokeRGB.g == groupChild.strokeRGB.b == strokeRGB.r == strokeRGB.g == strokeRGB.b)
                    strokeOpacity = 0;
            }
        }
        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        // Path
        Path path_;
        string pathValue = node.attribute("d").value();
        convertPathToValue(pathValue, path_);
        ClassPath* path = new ClassPath(fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, path_, fill, "", 0, 0);
        elements.push_back(path);
    }

    else if (nodeName == "g")
    {
        if (isParsingGroup)
        {
            return;
        }
        isParsingGroup = true;
        vector<Shape*> groupElements;
        RGB fillRGB, strokeRGB;
        smatch matches;

        float fontSize = node.attribute("font-size").as_float();
        // FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? 1
            : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255, 255, 255 };
        }
        convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);
        if (!fill.empty())
        {
            groupChild.fillRGB = fillRGB;
        }
        // STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? 1
            : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255, 255, 255 };
        }

        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
        if (!stroke.empty())
        {
            groupChild.strokeRGB = strokeRGB;
        }
        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth
            : node.attribute("stroke-width").as_float();

        // TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransform(transformValue, transform);
        // groupChild.transform = transform;
        groupChild.transform.translateX += transform.translateX;
        groupChild.transform.translateY += transform.translateY;
        groupChild.transform.rotateAngle += transform.rotateAngle;
        groupChild.transform.scaleX *= transform.scaleX;
        groupChild.transform.scaleY = transform.scaleY;

        groupChild.fillOpacity = fillOpacity;
        groupChild.strokeOpacity = strokeOpacity;
        groupChild.strokeWidth = strokeWidth;

        isParsingGroup = false;
        for (pugi::xml_node& childNode : node.children())
        {
            parseSVGNode(childNode, groupElements, groupChild);
        }
        Group* group = new Group(groupElements, strokeOpacity, fillOpacity, strokeRGB, fillRGB, strokeWidth, transform, fontSize, "", "");
        elements.push_back(group);
    }
}

void parseAndRenderSVG(const string& filePath, vector<Shape*>& elements, vector<Defs*>& elements_defs, ViewBox*& viewBox)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    float width_out = 0, height_out = 0;
    if (result)
    {
        pugi::xml_node svgNode = doc.child("svg");
        string viewboxValue = svgNode.attribute("viewBox").value();
        bool check_viewbox = true;
        float x = 0, y = 0, width_in = 0, height_in = 0;
        if (viewboxValue == "")
        {
            check_viewbox = 0;
        }
        else
        {
            istringstream iss(viewboxValue);
            iss >> x >> y >> width_in >> height_in;
        }
        width_out = svgNode.attribute("width").as_float();
        height_out = svgNode.attribute("height").as_float();

        *viewBox = ViewBox(x, y, width_in, height_in, width_out, height_out, check_viewbox);
        for (pugi::xml_node elementNode : svgNode.children())
        {
            string elementName = elementNode.name();

            if (elementName == "circle")
            {
                float cx = elementNode.attribute("cx").as_float();
                float cy = elementNode.attribute("cy").as_float();
                float r = elementNode.attribute("r").as_float();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill = "", stroke = "";
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fill = "";
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else if (fill.find("url") == string::npos) {
                        fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                    }
                    else if (!fill.empty()) {
                        int hashtagPos = fill.find("#");
                        int stopPos = fill.find(")");
                        fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                    }

                    // STROKE
                    stroke = elementNode.attribute("stroke").value();
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    if (stroke == "none" || stroke == "")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Circle* circle = new Circle(cx, cy, r, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, fill, "");
                elements.push_back(circle);
            }

            else if (elementName == "rect")
            {

                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                float width = elementNode.attribute("width").as_float();
                float height = elementNode.attribute("height").as_float();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fill = "";
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else if (fill.find("url") == string::npos)
                    {
                        fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                    }
                    else {
                        int hashtagPos = fill.find("#");
                        int stopPos = fill.find(")");
                        fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                    }

                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Rect_* rect = new Rect_(x, y, width, height, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, fill, "");
                elements.push_back(rect);
            }

            else if (elementName == "line")
            {
                float x1 = elementNode.attribute("x1").as_float();
                float y1 = elementNode.attribute("y1").as_float();
                float x2 = elementNode.attribute("x2").as_float();
                float y2 = elementNode.attribute("y2").as_float();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Line* line = new Line(x1, y1, x2, y2, strokeOpacity, strokeRGB, strokeWidth, transform, "");
                elements.push_back(line);
            }

            else if (elementName == "text")
            {
                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                float dx = elementNode.attribute("dx").as_float();
                float dy = elementNode.attribute("dy").as_float();
                string textAnchor = elementNode.attribute("text-anchor").empty()
                    ? "start"
                    : elementNode.attribute("text-anchor").value();
                string fontStyle = elementNode.attribute("font-style").empty()
                    ? "normal"
                    : elementNode.attribute("font-style").value();

                // Content extraction
                string content;
                for (pugi::xml_node child = elementNode.first_child(); child; child = child.next_sibling())
                {
                    if (child.type() == pugi::node_pcdata)
                    {
                        content = child.value();
                        break;
                    }
                }

                float fontSize = elementNode.attribute("font-size").as_float();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;
                bool checkk = true;
                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else
                {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fill = "";
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else if (fill.find("url") == string::npos) {
                        fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                    }
                    else if (!fill.empty()) {
                        int hashtagPos = fill.find("#");
                        int stopPos = fill.find(")");
                        fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                    }

                    // STROKE
                    checkk = true;
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeOpacity = 1;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else if (stroke == "")
                    {
                        strokeRGB = { 255, 255, 255 };
                        checkk = 0;
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);

                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                Transform trans = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                // fontFamily
                string fontFamily = elementNode.attribute("font-family").empty()
                    ? "Times New Roman"
                    : elementNode.attribute("font-family").value();

                Text* text = new Text(x, y, content, fontSize, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fontFamily, dx, dy, textAnchor, fontStyle, checkk, fill, "", trans);
                elements.push_back(text);
            }
            //2 cai poly ko co lam gradient
            else if (elementName == "polyline")
            {
                string points = elementNode.attribute("points").value();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fillOpacity = 0;
                        fillRGB = { 256, 256, 256 };
                    }
                    else
                    {
                        convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);
                    }

                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Polyline_* polyline = new Polyline_(points, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, "", "");
                elements.push_back(polyline);
            }

            else if (elementName == "polygon")
            {
                string points = elementNode.attribute("points").value();

                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);

                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Polygon_* polygon = new Polygon_(points, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform, "", "");
                elements.push_back(polygon);
            }

            else if (elementName == "ellipse")
            {
                float cx = elementNode.attribute("cx").as_float();
                float cy = elementNode.attribute("cy").as_float();
                float rx = elementNode.attribute("rx").as_float();
                float ry = elementNode.attribute("ry").as_float();
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }



                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fill = "";
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else if (fill.find("url") == string::npos)
                    {
                        fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                    }
                    else if (!fill.empty()) {
                        int hashtagPos = fill.find("#");
                        int stopPos = fill.find(")");
                        fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                    }

                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }

                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Ellipse_* ellipse = new Ellipse_(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fill, "");
                elements.push_back(ellipse);
            }

            else if (elementName == "path")
            {
                float fillOpacity = 1, strokeOpacity = 1, strokeWidth = 1;
                string fill, stroke;
                RGB fillRGB = { 0, 0, 0 };
                RGB strokeRGB = { 0, 0, 0 };
                smatch matches;

                string styleString = elementNode.attribute("style").value();
                if (!styleString.empty()) {
                    convertStyle(styleString, fill, stroke, fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth);
                }
                else {
                    // FILL
                    fillOpacity = elementNode.attribute("fill-opacity").empty()
                        ? 1
                        : elementNode.attribute("fill-opacity").as_float();
                    fill = elementNode.attribute("fill").value();
                    if (fill == "none")
                    {
                        fill = "";
                        fillOpacity = 0;
                        fillRGB = { 255, 255, 255 };
                    }
                    else if (fill.find("url") == string::npos) {
                        fill = convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
                    }
                    else if (!fill.empty()) {
                        int hashtagPos = fill.find("#");
                        int stopPos = fill.find(")");
                        fill = fill.substr(hashtagPos + 1, stopPos - hashtagPos - 1);
                    }

                    // STROKE
                    strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                        ? 1
                        : elementNode.attribute("stroke-opacity").as_float();
                    stroke = elementNode.attribute("stroke").value();
                    if (stroke == "none" || stroke == "")
                    {
                        strokeOpacity = 0;
                        strokeRGB = { 255, 255, 255 };
                    }
                    else
                        convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                    strokeWidth = elementNode.attribute("stroke-width").empty()
                        ? 1
                        : elementNode.attribute("stroke-width").as_float();
                }
                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                // Path
                Path path_;
                string pathValue = elementNode.attribute("d").value();
                convertPathToValue(pathValue, path_);
                ClassPath* path = new ClassPath(fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, path_, fill, "", height_out, width_out);
                elements.push_back(path);
            }

            else if (elementName == "g")
            {
                RGB fillRGB, strokeRGB;
                groupChild groupChild_;
                smatch matches;

                // FONT SIZE
                float fontSize = elementNode.attribute("font-size").as_float();

                // FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1
                    : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 256, 256, 256 };
                }
                else
                    convert_String_to_RGB_(fillRGB, fill, matches, rgbRegex);

                // STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1
                    : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                convert_String_to_RGB_(strokeRGB, stroke, matches, rgbRegex);
                if (stroke == "")
                {
                    bool checked = false;
                    checkRGB.push_back(checked);
                }
                else
                {
                    bool checked = true;
                    checkRGB.push_back(checked);
                }
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1
                    : elementNode.attribute("stroke-width").as_float();

                // TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                // push group child elements
                groupChild_.fillOpacity = fillOpacity;
                groupChild_.strokeOpacity = strokeOpacity;
                groupChild_.fillRGB = fillRGB;
                groupChild_.strokeRGB = strokeRGB;
                groupChild_.strokeWidth = strokeWidth;
                groupChild_.transform = transform;
                groupChild_.fontSize = fontSize;

                vector<Shape*> shapes;
                int i = 0;
                // Duyệt qua các node con bên trong group
                for (pugi::xml_node& childNode : elementNode.children())
                {
                    parseSVGNode(childNode, shapes, groupChild_);
                    isParsingGroup = false;
                }
                Group* group = new Group(shapes, strokeOpacity, fillOpacity, strokeRGB, fillRGB, strokeWidth, transform, fontSize, fill, stroke);
                elements.push_back(group);
            }

            else if (elementName == "defs") {
                vector<LinearGradient*> lg_list;
                vector<RadialGradient*> rg_list;
                for (pugi::xml_node& childNode : elementNode.children())
                {
                    string nodeName = childNode.name();
                    if (nodeName == "linearGradient")
                    {
                        vector<Stop*> stop_list;
                        pointLinearGradient point;
                        string styleString = childNode.attribute("style").value();
                        string id, gradientUnits, spreadMethod;
                        if (!styleString.empty()) {
                            convertStyleGradient(styleString, id, gradientUnits, spreadMethod);
                        }
                        else {
                            id = childNode.attribute("id").value();
                            gradientUnits = childNode.attribute("gradientUnits").value();
                            spreadMethod = childNode.attribute("spreadMethod").value();
                        }

                        string transformValue = childNode.attribute("gradientTransform").value();
                        Transform transform = { 0, 0, 0, 1.0, 1.0, 0, 0 };

                        if (transformValue.find("matrix") != string::npos) {
                            parseTransformMatrix(transformValue, transform);
                        }
                        else
                        {
                            parseTransform(transformValue, transform);
                        }

                        point.x1 = childNode.attribute("x1").as_float();
                        point.x2 = childNode.attribute("x2").as_float();
                        point.y1 = childNode.attribute("y1").as_float();
                        point.y2 = childNode.attribute("y2").as_float();
                        for (pugi::xml_node& grandNode : childNode.children()) {
                            string grandName = grandNode.name();
                            if (grandName == "stop") {
                                float offset = grandNode.attribute("offset").empty()
                                    ? 0 : grandNode.attribute("offset").as_float();
                                float stopOpacity = grandNode.attribute("stop-opacity").empty()
                                    ? 1 : grandNode.attribute("stop-opacity").as_float();
                                RGB stopColor;
                                smatch matches;
                                string stopColorValue = grandNode.attribute("stop-color").value();
                                convert_String_to_RGB(stopColor, stopColorValue, matches, rgbRegex);
                                Stop* stop = new Stop(offset, stopOpacity, stopColor);
                                stop_list.push_back(stop);
                            }
                        }

                        LinearGradient* linearGradient = new LinearGradient(id, point, stop_list, transform);
                        lg_list.push_back(linearGradient);
                    }

                    else if (nodeName == "radialGradient") {
                        vector<Stop*> stop_list;
                        string id = childNode.attribute("id").value();
                        float cx = childNode.attribute("cx").as_float();
                        float cy = childNode.attribute("cy").as_float();
                        float r = childNode.attribute("r").as_float();

                        string gradientUnits = childNode.attribute("gradientUnits").value();

                        string xlink_href_temp = childNode.attribute("xlink:href").value();
                        int hashtagPos = xlink_href_temp.find("#");
                        string xlink_href = xlink_href_temp.substr(hashtagPos + 1);

                        string transformValue = childNode.attribute("gradientTransform").value();
                        Transform transform = { 0, 0, 0, 1.0, 1.0 };

                        if (transformValue.find("matrix") != string::npos)
                        {
                            parseTransformMatrix(transformValue, transform);
                        }
                        else {
                            parseTransform(transformValue, transform);
                        }

                        float fx = childNode.attribute("fx").as_float();
                        float fy = childNode.attribute("fy").as_float();

                        for (pugi::xml_node& grandNode : childNode.children())
                        {
                            string grandName = grandNode.name();
                            if (grandName == "stop") {
                                float offset = grandNode.attribute("offset").empty()
                                    ? 0 : grandNode.attribute("offset").as_float();
                                float stopOpacity = grandNode.attribute("stop-opacity").empty()
                                    ? 1 : grandNode.attribute("stop-opacity").as_float();
                                RGB stopColor;
                                smatch matches;
                                string stopColorValue = grandNode.attribute("stop-color").value();
                                convert_String_to_RGB(stopColor, stopColorValue, matches, rgbRegex);
                                Stop* stop = new Stop(offset, stopOpacity, stopColor);
                                stop_list.push_back(stop);
                            }
                        }

                        RadialGradient* radialGradient = new RadialGradient(id, cx, cy, r, xlink_href, transform, fx, fy, stop_list);
                        rg_list.push_back(radialGradient);
                    }
                }
                Defs* defs = new Defs(lg_list, rg_list);
                elements_defs.push_back(defs);
            }
        }
    }
}