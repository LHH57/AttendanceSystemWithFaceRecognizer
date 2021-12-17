import os
if __name__ == "__main__":
    f = open("pics.csv", mode="w")
    files = os.listdir(os.getcwd())
    for file in files:
        if os.path.isdir(file) and file.isdecimal():
            pics = os.listdir(file)
            f.writelines([os.path.join(os.getcwd(), file, pic) + ";" + file + "\n" for pic in pics])
            print(f"face index = {file}, pics num = {len(pics)}")
    f.close()
