#include "ini/SimpleIni.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

struct TxtFile {
  std::string path;
  std::string data = "";
};

struct IniFile {
  std::string path;
};

struct JsonFile {
  std::string path;
  Json::Value data;
};

class FileTools {
private:
  std::vector<std::string> split(const std::string &data,
                                 const std::string &separator) {
    std::vector<std::string> result;
    if (data == "")
      return result;

    char *thisStr = new char[data.length() + 1];
    strcpy(thisStr, data.c_str());

    char *thisSeparator = new char[separator.length() + 1];
    strcpy(thisSeparator, separator.c_str());

    char *token = strtok(thisStr, thisSeparator);
    while (token) {
      std::string tempStr = token;
      result.push_back(tempStr);
      token = strtok(NULL, thisSeparator);
    }

    return result;
  }

public:
  std::string get_current_directory() {
    char buff[250];
    getcwd(buff, 250);
    std::string current_working_directory(buff);
    return current_working_directory;
  }

#pragma region txt

  bool readTxtFileLine(TxtFile &txtFile) {
    std::string ln;
    std::fstream file;

    file.open(txtFile.path, std::ios::in);

    if (file.is_open()) {
      while (std::getline(file, ln))
        txtFile.data += (ln + "\n");

      file.close();

      return true;
    } else
      return false;
  };

  bool writeDataToTxtFile(TxtFile &txtFile, std::string data) {
    std::fstream file;

    file.open(txtFile.path, std::ios::out);

    if (file.is_open()) {
      file << data;
      txtFile.data = data;

      file.close();

      return true;
    } else
      return false;
  };

#pragma endregion

#pragma region ini

  bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  std::string &param, const char *defaultVal) {
    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    param = ini.GetValue(section, key, defaultVal);

    return true;
  };

  template <typename T>
  bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  T &param, T defaultVal) {
    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    const char *name = typeid(T).name();
    std::string paramType = name;
    std::string tempParam;
    tempParam = ini.GetValue(section, key, std::to_string(defaultVal).c_str());

    if (paramType == "i")
      param = std::stoi(tempParam);
    else if (paramType == "f")
      param = std::stof(tempParam);
    else if (paramType == "d")
      param = std::stod(tempParam);
    else if (paramType == "b")
      if (tempParam == "false" || tempParam == "0") {
        param = false;
      } else if (tempParam == "true" || tempParam == "1") {
        param = true;
      }

    return true;
  };

  template <typename T>
  bool getFromIni(const IniFile &iniFile, const char *section, const char *key,
                  T *param, T *defaultVal, const int &size) {
    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    int index = 0;

    const char *name = typeid(T).name();
    std::string paramType = name;

    if (ini.GetValue(section, key) == nullptr)
      while (index <= size - 1) {
        param[index] = defaultVal[index];
        index++;
      }
    else {
      std::string tempParamArrayStr = ini.GetValue(section, key);
      std::vector<std::string> tempParamArray =
          split(tempParamArrayStr, " ,\t\n");

      if (paramType == "i")
        for (int i = 0; i < tempParamArray.size(); ++i) {
          param[index++] = std::stoi(tempParamArray[i]);
        }
      else if (paramType == "f")
        for (int i = 0; i < tempParamArray.size(); ++i) {
          param[index++] = std::stof(tempParamArray[i]);
        }
      else if (paramType == "d")
        for (int i = 0; i < tempParamArray.size(); ++i) {
          param[index++] = std::stod(tempParamArray[i]);
        }

      while (index <= size - 1) {
        param[index] = defaultVal[index];
        index++;
      }
    }

    return true;
  };

  bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                const char *fromValue) {
    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    std::string toValue = fromValue;
    const char *toValueC = (char *)toValue.c_str();

    rc = ini.SetValue(section, key, toValueC);
    if (rc < 0)
      return false;

    std::string output;
    ini.Save(output);

    rc = ini.SaveFile(path);
    if (rc < 0)
      return false;

    return true;
  };

  template <typename T>
  bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                T fromValue) {
    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    const char *name = typeid(T).name();
    std::string valueType = name;
    std::string toValue;

    if (valueType == "i")
      toValue = std::to_string(fromValue);
    else if (valueType == "f")
      toValue = std::to_string(fromValue);
    else if (valueType == "d")
      toValue = std::to_string(fromValue);
    else if (valueType == "b")
      if (fromValue == false) {
        toValue = "false";
      } else if (fromValue == true) {
        toValue = "true";
      }

    const char *toValueC = (char *)toValue.c_str();

    rc = ini.SetValue(section, key, toValueC);
    if (rc < 0)
      return false;

    std::string output;
    ini.Save(output);

    rc = ini.SaveFile(path);
    if (rc < 0)
      return false;

    return true;
  };

  template <typename T>
  bool setToIni(const IniFile &iniFile, const char *section, const char *key,
                T *fromValueArr, const int &size) {
    if (size <= 0)
      return false;

    CSimpleIniA ini;
    ini.SetUnicode();

    const char *path = (char *)iniFile.path.c_str();

    SI_Error rc = ini.LoadFile(path);
    if (rc < 0)
      return false;

    const char *name = typeid(T).name();
    std::string valueType = name;
    std::string toValueArr;

    if (valueType == "i")
      for (int i = 0; i < size; ++i) {
        toValueArr += std::to_string(fromValueArr[i]);
        if (i != size - 1) {
          toValueArr += ", ";
        }
      }
    else if (valueType == "f")
      for (int i = 0; i < size; ++i) {
        toValueArr += std::to_string(fromValueArr[i]);
        if (i != size - 1) {
          toValueArr += ", ";
        }
      }
    else if (valueType == "d")
      for (int i = 0; i < size; ++i) {
        toValueArr += std::to_string(fromValueArr[i]);
        if (i != size - 1) {
          toValueArr += ", ";
        }
      }

    const char *toValueC = (char *)toValueArr.c_str();

    rc = ini.SetValue(section, key, toValueC);
    if (rc < 0)
      return false;

    std::string output;
    ini.Save(output);

    rc = ini.SaveFile(path);
    if (rc < 0)
      return false;

    return true;
  };

#pragma endregion

#pragma region json

  bool readDataFromJsonFile(JsonFile &jsonFile) {
    std::fstream file;

    file.open(jsonFile.path, std::ios::in);

    if (file.is_open()) {
      Json::CharReaderBuilder ReaderBuilder;
      ReaderBuilder["emitUTF8"] = true;

      Json::Value root;

      std::string strerr;

      bool flag = Json::parseFromStream(ReaderBuilder, file, &root, &strerr);

      if (!flag) {
        std::cout << strerr << std::endl;
        return false;
      }

      jsonFile.data = root;

      file.close();

      return true;
    } else
      return false;
  }

  void getFromJsonData(const JsonFile &jsonFile, const std::string &key,
                       std::string &param, std::string defaultVal) {
    Json::Value temp = jsonFile.data;
    std::vector<std::string> keyArr = split(key, ".");

    for (int i = 0; i < keyArr.size() - 1; ++i)
      temp = temp[keyArr[i]];

    param = temp.get(keyArr[keyArr.size() - 1], defaultVal).asString();
  }

  template <typename T>
  void getFromJsonData(const JsonFile &jsonFile, const std::string &key,
                       T &param, T defaultVal) {
    Json::Value temp = jsonFile.data;
    std::vector<std::string> keyArr = split(key, ".");
    const char *name = typeid(T).name();
    std::string valueType = name;

    for (int i = 0; i < keyArr.size() - 1; ++i)
      temp = temp[keyArr[i]];

    if (valueType == "i")
      param = temp.get(keyArr[keyArr.size() - 1], defaultVal).asInt();
    else if (valueType == "d")
      param = temp.get(keyArr[keyArr.size() - 1], defaultVal).asDouble();
    else if (valueType == "b")
      param = temp.get(keyArr[keyArr.size() - 1], defaultVal).asBool();
  }

  void getFromJsonData(const JsonFile &jsonFile, const std::string &key,
                       std::string *param, std::string *defaultVal,
                       const int &size) {
    Json::Value temp = jsonFile.data;
    std::vector<std::string> keyArr = split(key, ".");

    for (int i = 0; i < keyArr.size() - 1; ++i)
      temp = temp[keyArr[i]];

    const Json::Value thisKeyArrData = temp[keyArr[keyArr.size() - 1]];
    int index = 0;

    for (int i = 0; i < thisKeyArrData.size(); ++i)
      param[index++] = thisKeyArrData[index].asString();

    if (index < size) {
      for (int i = index; i < size; ++i)
        param[i] = defaultVal[i];
    }
  }

  template <typename T>
  void getFromJsonData(const JsonFile &jsonFile, const std::string &key,
                       T *param, T *defaultVal, const int &size) {
    Json::Value temp = jsonFile.data;
    std::vector<std::string> keyArr = split(key, ".");
    const char *name = typeid(T).name();
    std::string valueType = name;

    for (int i = 0; i < keyArr.size() - 1; ++i)
      temp = temp[keyArr[i]];

    const Json::Value thisKeyArrData = temp[keyArr[keyArr.size() - 1]];
    int index = 0;

    if (valueType == "i")
      for (int i = 0; i < thisKeyArrData.size(); ++i)
        param[index++] = thisKeyArrData[index].asInt();
    else if (valueType == "d")
      for (int i = 0; i < thisKeyArrData.size(); ++i)
        param[index++] = thisKeyArrData[index].asDouble();

    if (index < size) {
      for (int i = index; i < size; ++i)
        param[i] = defaultVal[i];
    }
  }

#pragma endregion
};
