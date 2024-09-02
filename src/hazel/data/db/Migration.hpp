#include "SQLiteCpp/Database.h"
#include <string>
#include <vector>

namespace hazel {

/**
 * Note: this is just a version control wrapper. The queries and migration controls are still
 * manually managed. This is so I don't have to spend an eterity writng this clusterfuck,
 * because there are an extreme number of edge-cases in migrations. There's defaults to consider when
 * rows are added to an existing table, different col types, etc. It's far easier to build
 * it for just query versioning.
 */
class Migration {
private:
    std::vector<std::string> queries;

    /**
     * Used to provide an offset to the versions 
     *
     * This is currently not possible to set anywhere,
     * and is kept purely for a hypothetical future
     * scenario, because why the fuck not? Let's
     * overengineer this class :D
     */
    constexpr static int64_t versionOffset = 0;
public:
    Migration() {}
    
    Migration& pushVersion(const std::string& query);
    void exec(SQLite::Database& db);
    
    static void prepMetatables(SQLite::Database& db);

};
}
