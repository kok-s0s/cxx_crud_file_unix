# CXX_CRUD_FILE_UNIX

## txt

```cxx
struct TxtFile {
  std::string path;
  std::string data = "";
};

bool readTxtFileLine(TxtFile &txtFile);

bool writeDataToTxtFile(TxtFile &txtFile, std::string data);
```

## ini

Based On `SimpleIni` --- [Github](https://github.com/brofield/simpleini)

```cxx
struct IniFile {
  std::string path;
};

bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  std::string &param, const char *defaultVal);

template <typename T>
bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  T &param, T defaultVal);

template <typename T>
bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  T *param, T *defaultVal, const int &size);

bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                const char *fromValue);

template <typename T>
bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                T fromValue);

template <typename T>
bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                T *fromValueArr, const int &size);
```

## json

Based On `jsoncpp` --- [Github](https://github.com/open-source-parsers/jsoncpp)

```cxx
struct JsonFile {
  std::string path;
  Json::Value data;
};

bool readDataFromJsonFile(JsonFile &jsonFile);

void getFromJson(const JsonFile &jsonFile, const std::string &name,
                   std::string &param, std::string defaultVal);
```
