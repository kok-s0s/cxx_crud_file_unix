# CXX_CRUD_FILE_UNIX

## Development environment

**Available C++ Compilers**

- `GCC 9.4.0 x86_64-linux-gnu`
- `GCC 11.2.0 aarch64-apple-darwin21`

**VSCode extensions**

- `C/C++`
- `CMake`
- `CMake Tools`
- `clang-format`
- `Hex Editor`

## Test-Driven Development (TDD)

[GoogleTest](http://google.github.io/googletest/quickstart-cmake.html)

## Function

### txt

```cxx
struct TxtFile {
  string path;
  string data = "";
};

bool readTxtFileLine(TxtFile &txtFile);

bool writeDataToTxtFile(TxtFile &txtFile, string data);
```

### ini

Based On `simpleini` --- [Github](https://github.com/brofield/simpleini)

```cxx
struct IniFile {
  string path;
};

bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                string &param, const char *defaultVal);

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

### json

Based On `json` --- [Github](https://github.com/nlohmann/json)

```cxx
struct JsonFile {
  string path;
  json data;
};

bool readDataFromJsonFile(JsonFile &jsonFile);

void getFromJsonData(const JsonFile &jsonFile, const string &key,
                     string &param, string defaultVal);

template <typename T>
void getFromJsonData(const JsonFile &jsonFile, const string &key, T &param,
                     T defaultVal);

void getFromJsonData(const JsonFile &jsonFile, const string &key,
                     string *param, string *defaultVal, const int &size);

template <typename T>
void getFromJsonData(const JsonFile &jsonFile, const string &key, T *param,
                     T *defaultVal, const int &size);
```

### dat

```cxx
struct DatFile {
  string path;
  vector<char> data;
};

bool readDatFile(DatFile &datFile);

bool readDatFile(DatFile &datFile, char *varibale, const int &num);

bool writeDataToDatFile(const DatFile &datFile);
```