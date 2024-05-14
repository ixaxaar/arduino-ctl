struct FunctionInfo
{
    std::string name;
    std::vector<std::pair<std::string, std::string>> params;
};

class ModuleInterface
{
public:
    virtual void init(const std::vector<std::pair<std::string, std::string>> &params) = 0;
    virtual void deinit() = 0;
    virtual std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) = 0;
    virtual std::vector<FunctionInfo> getSupportedFunctions() = 0;
};
