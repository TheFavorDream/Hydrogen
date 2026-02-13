import os
import shutil 

Root = "./"

for dir in os.scandir(Root):
    if dir.is_dir() and dir.name.endswith("_tests"):
        print(f"Removing {dir.name}")
        shutil.rmtree(dir.path+"/obj", ignore_errors=True)
        shutil.rmtree(dir.path+"/bin", ignore_errors=True)

