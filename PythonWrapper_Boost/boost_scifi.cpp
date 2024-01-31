#include "../backend/include/duckstore.h"
#include "../backend/include/rocksstore.h"
#include "../framework/include/storage.h"
#include <string>
#include <vector>
#include <boost/python/raw_function.hpp>
#include <boost/python.hpp>


#define TEMPLATE(args) SciStore::Storage<args>
#define TEMPLATE_ARGS SciStore::RocksStore, SciStore::DuckStore

typedef TEMPLATE(TEMPLATE_ARGS) Storage_template;

//Boost Function creating thin wrapper for overloading functions with default parameter
//Cave:: Overloading for Default parameters is handled only for positional Arguments, keyword arguments have do be defined in .def in Python module
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(load_assets_from_dir_overloads, Storage_template::load_assets_from_directory,1,6)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_id_overloads, Storage_template::get, 1,6)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_ids_overloads,Storage_template::get, 1,6)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(load_asset_from_file_overloads, Storage_template::load_asset_from_file,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(load_metadata_advanced_overloads, Storage_template::load_metadata_advanced,3,4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(create_empty_metadata_table_overloads, Storage_template::create_empty_metadata_table,1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(load_metadata_advanced_from_remote_overloads, Storage_template::load_metadata_advanced_from_remote,3,4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_by_constraint_overloads, Storage_template::get_by_constraint, 1,5)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(apply_filter_overloads, Storage_template::apply_filter,1,5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_all_assets_overloads, Storage_template::get_all_assets,0,5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(remove_asset_overloads, Storage_template::remove_asset, 1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(define_user_meta_cols_overloads, Storage_template::define_user_meta_cols, 1,2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(insert_new_meta_row_overloads, Storage_template::insert_new_meta_row,1,2)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(open_meta_store_overloads, Storage_template::open_meta_store, 1,2)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_wrapper_overload, Storage_template::wrapper_get_function, 1,6)
//Function pointer for overload functions with same name, but different datatypes
//Cave: If the functions do have default parameters, boost_python_member_function_overloads is also needed

std::string (Storage_template::*get_id)(std::string, int, std::string, int, std::string, int)= &Storage_template::get;
std::vector<std::string> (Storage_template::*get_ids)(std::vector <std::string>s, int, std::string, int, std::string, bool)= &Storage_template::get;


//to_python converter
struct PyString_to_python
{
    static PyObject* convert(PyString const& cpp_string)
        {std::cout<< "string conversion" << std::endl;
        std::string string = cpp_string.cpp_string;
        std::string py_string = cpp_string.cpp_string;
        const char* py_strings = py_string.data();
        std::cout << py_strings << std::endl;
        PyObject* result = PyBytes_FromString(py_strings);

        return boost::python::incref(result);
    }
};

struct Vector_to_python_list
{
    static PyObject* convert(std::vector<std::string> const& vec)
    {
        boost::python::list list;
        typename std::vector<std::string>::const_iterator iterator;
        for (iterator=vec.begin(); iterator<vec.end(); iterator++)
        {
            list.append(*iterator);
        }
        std::cout << "vector conversion" << std::endl;

        return boost::python::incref(list.ptr());
    }
};
/*
struct string_from_string{
    string_from_string(){boost::python::converter::registry::push_back(&convertible,&construct,boost::python::type_id<std::string>());}
    static void* convertible(PyObject* obj_ptr){
        if(!PyUnicode_Check(obj_ptr)) return 0;
        return obj_ptr;
    }
    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data){
        const char* string_value = PyBytes_AsString(obj_ptr);

        void* storage = (
                (boost::python::converter::rvalue_from_python_storage<std::string>*)
                        data)->storage.bytes;

        new (storage) std::string(string_value);

        data->convertible = storage;
        std::cout << "custom string conversion" << std::endl;
    }

};*/
struct vector_from_seq{
    vector_from_seq(){ boost::python::converter::registry::push_back(&convertible,&construct,boost::python::type_id<std::vector<std::string> >()); }
    static void* convertible(PyObject* obj_ptr){
        // the second condition is important, for some reason otherwise there were attempted conversions of Body to list which failed afterwards.
        if(!PySequence_Check(obj_ptr) or !PyObject_HasAttrString(obj_ptr,"__len__") or PyUnicode_Check(obj_ptr)) return 0;
        return obj_ptr;
    }
    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data){
        void* storage=((boost::python::converter::rvalue_from_python_storage<std::vector<std::string> >*)(data))->storage.bytes;
        new (storage) std::vector<std::string>();
        std::vector<std::string>* v=(std::vector<std::string>*)(storage);
        int l=PySequence_Size(obj_ptr); if(l<0) abort();
        v->reserve(l);
        for(int i=0; i<l; i++)
        { v->push_back(boost::python::extract<std::string>(PySequence_GetItem(obj_ptr,i)));
        }
        data->convertible=storage;
    }
};


using namespace boost::python;

BOOST_PYTHON_MODULE(boost_scifi){


//register converter

to_python_converter<PyString, PyString_to_python>();
to_python_converter<std::vector<std::string>, Vector_to_python_list>();
vector_from_seq();
//string_from_string();

class_<Storage_template>("Storage", init <std::string, std::string, optional<std::string, int>>())

    .def ("load_assets_from_dir",&Storage_template::load_assets_from_directory, load_assets_from_dir_overloads(
            (arg("dir"), arg("portable")= 0, arg("recursive")= 0, arg("depth")= 0, arg("ext")= 0, arg("prefix")=0)))
    .def ("load_asset_from_file", &Storage_template::load_asset_from_file,load_asset_from_file_overloads(
            (arg("file"), arg("directory")="")))
    .def("load_asset_from_remote", &Storage_template::load_asset_from_remote,arg("address"))
    .def("import_asset_store", &Storage_template::import_asset_store,arg("file"))

    .def("get",get_id, get_id_overloads(
            (arg("id"), arg("assetToFile")= 0, arg("fileextension") = "", arg("metaToFile") = 0, arg("fileextension_meta") = "", arg("dumpmeta")=0)))
    .def("get", get_ids, get_ids_overloads(
            (arg("id"), arg("assetToFile")= 0, arg("fileextension") = "", arg("metaToFile") = 0, arg("fileextension_meta") = "", arg("join_metatables")=true)))
    .def("get_by_constraint", &Storage_template::get_by_constraint,get_by_constraint_overloads(
            (arg("constraint"), arg("assetToFile")= 0, arg("fileextension") = "", arg("metaToFile") = 0, arg("fileextension_meta") = "")))
    .def("get_IDs_by_constraint", &Storage_template::get_IDs_by_constraint, arg("constraint"))
    .def("get_all_assets", &Storage_template::get_all_assets,get_all_assets_overloads(
            (arg("assetToFile")=0, arg("fileextension")="", arg("metaToFile")=0, arg("fileextension_meta")="", arg("ids_only")=0)))

    .def("remove_asset", &Storage_template::remove_asset, remove_asset_overloads(
            (arg("id"), arg("remove_metadata")=0)))

    .def("store_filter", &Storage_template::store_filter, (arg("constraint"), arg("filtername"), arg("filtertext")))
    .def("apply_filter", &Storage_template::apply_filter, apply_filter_overloads(
            (arg("filtername"), arg("assetToFile")= 0, arg("fileextension") = "", arg("metaToFile") = 0, arg("fileextension_meta") = "")))
    .def("remove_filter", &Storage_template::remove_filter, arg("filtername"))
    .def("get_all_filters", &Storage_template::get_all_filters)
    .def("get_filter_text", &Storage_template::get_filter_text, arg("filtername"))
    

    .def("open_meta_store", &Storage_template::open_meta_store, open_meta_store_overloads(
            (arg("metaDBname"), arg("resultsfolder")="results")))
    .def ("load_metadata_from_file", &Storage_template::load_metadata_from_file, (arg("file"), arg("IDCol")))
    .def("load_metadata_from_remote", &Storage_template::load_metadata_from_remote)
    .def("load_metadata_advanced_from_file", &Storage_template::load_metadata_advanced, load_metadata_advanced_overloads(
            (arg("file"), arg("skiplines"), arg("columns"), arg("IDColumn")=0)))
    .def("load_metadata_advanced_from_remote", &Storage_template::load_metadata_advanced_from_remote,load_metadata_advanced_from_remote_overloads(
            (arg("address"), arg("skiplines"), arg("columns"), arg("IDColumn")=0)))

    .def("add_meta_table", &Storage_template::add_meta_table, (arg("tablename"), arg("foreign_key"), arg("columns")))
    .def("add_meta_table_advanced", &Storage_template::add_meta_table_advanced,(arg("tablename"), arg("foreign_key"), arg("file"), arg("skiplines"), arg("columns")))
    .def("create_empty_metadata_table", &Storage_template::create_empty_metadata_table,create_empty_metadata_table_overloads(
            (arg("columns"), arg("IDColumn")=0)))
    .def("define_user_meta_cols", &Storage_template::define_user_meta_cols, define_user_meta_cols_overloads(
            (arg("coldef"), arg("IDColumn")=0)))
    .def("insert_new_meta_row", &Storage_template::insert_new_meta_row,insert_new_meta_row_overloads(
            (arg("values"), arg("tablename")="metadata")))

    .def("asset_count", &Storage_template::asset_count)
    .def("get_item", &Storage_template::getitem)
    .def("get_IDsByIndex", &Storage_template::getIDsByIndex)

    .def("get_wrapped", &Storage_template::wrapper_get_function, get_wrapper_overload(
        (arg("id"), arg("assetToFile")= 0, arg("fileextension") = "", arg("metaToFile") = 0, arg("fileextension_meta") = "", arg("dumpmeta")=0)));
}