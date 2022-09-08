from distutils.core import setup, Extension



setup (
	   name = "py_lib",
	   version = "0.1",
	   author = "Marijn",
	   description = """module based on C++ compiled with swig""",
	   ext_modules = [
		   Extension(
				"_py_lib",
				sources=[
					"src\\hash.cpp",
					"int\\hash.cpp",
					"src\\encryption.cpp",
					"int\\encryption.cpp",
					"src\\io.cpp",
					"int\\io.cpp",
					# final cpp / hpp files
					"int\\py_lib.cpp",
				]
		   )
	   ],
	   py_modules = ["py_lib"],
	   swig_version="swigwin-4.0.2",
)