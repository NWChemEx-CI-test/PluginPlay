"""
This file contains helper functions used throughout the various parsing scripts
in this directory
"""

# Writes warning about auto-generation to file
def write_warning(script_name, f):
    f.write("/* Warning!!!! \n")
    f.write(" * This file was auto-generated by {} ".format(script_name))
    f.write("and should not be\n")
    f.write(" * modified.  To make modifications to this file please edit\n")
    f.write(" * {} and regenerate this file.\n".format(script_name))
    f.write(" */\n")

def write_header(script_name, f, nmspace="SDE"):
    f.write("#include \"SDE/NWXDefaults.hpp\"\n")
    write_warning(script_name, f)
    f.write("namespace {} {{\n".format(nmspace))

def write_footer(f, nmspace="SDE"):
    f.write("    return rv;\n")
    f.write("}}\n}} // End namespace {}\n".format(nmspace))
