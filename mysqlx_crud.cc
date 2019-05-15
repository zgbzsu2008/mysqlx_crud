#include <assert.h>
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using namespace mysqlx;

struct
{
  std::string host = "localhost";
  std::string user = "root";
  std::string pwd = "353213";
  std::string db = "mydb";
  unsigned short port = 33060;
} config;

void printSchema(Session &sess) {}

int main(int argc)
{
  try {
    Session sess("localhost", 33060, "root", "353213");
    {
      // get version
      RowResult res = sess.sql("show variables like 'version'").execute();
      std::stringstream version;

      version << res.fetchOne().get(1).get<string>();
      int major_version;
      version >> major_version;
      if (major_version < 8) {
        cout << "major_version < 8!" << endl;
        return 0;
      }
    }
    {
      // get all schema
      std::vector<Schema> schemas = sess.getSchemas();
      cout << "schemas name: " << endl;
      for (unsigned i = 0; i < schemas.size(); ++i) {
        std::cout << "#" << i << ": " << schemas[i].getName() << endl;
      }
    }

    cout << "Session accepted, creating collection..." << endl;
    Schema sch = sess.getSchema("mydb", true);
    Collection coll = sch.createCollection("mycoll", true);
    coll.remove("true").execute();
    {
      // add
      Result res;
      res = coll.add(R"({"name": "zhangsan", "sex": 0, "age": 18, "birth": "1989-08-25"})")
                .add(R"({"name": "lisi", "sex": 1, "age": 28, "birth": "1999-08-25"})")
                .execute();
      // Warning
      std::vector<Warning> warnings = res.getWarnings();
      for (auto w : warnings) {
        std::cout << w << endl;
      }
      // getGeneratedIds
      std::vector<string> indices = res.getGeneratedIds();
      for (auto id : indices) {
        std::cout << "_id= " << id << endl;
      }
      // getAffectedItemsCount
      cout << res.getAffectedItemsCount() << endl;

      coll.remove("sex==0 or sex==1").execute();
    }
  } catch (const mysqlx::Error &err) {
    cout << "ERROR: " << err << endl;
    return 1;
  } catch (std::exception &ex) {
    cout << "STD EXCEPTION: " << ex.what() << endl;
    return 1;
  } catch (const char *ex) {
    cout << "EXCEPTION: " << ex << endl;
    return 1;
  }
}