import os
import sys

INFO_FILE_PATH = ".\\Build_tools\\Auto_Compiler\\info.txt"
MOD_TIME_INFO_FILE = ".\\tools\\Auto_Compiler\\times.txt"

MAX_DEPTH = 1

def main():
	os.system(f"make auto_info > {INFO_FILE_PATH}")

	with open(INFO_FILE_PATH, "r") as f:
		tmp = f.readline().strip()

		if tmp.startswith("make"):
			COMPILER_FLAGS = f.readline().strip()
		else:
			COMPILER_FLAGS = tmp # type: ignore
		
		BUILD_OBJ_PATH = f.readline().strip()
		BUILD_LOG_PATH = f.readline().strip()

	c_files: list[tuple[str, str]] = []
	h_files: list[tuple[str, str]] = []
	curr_mod_times: list[str] = []
	
	if sys.argv[1] == "full":
		for (root,_,files) in os.walk('.\\source', topdown=True): 
			if root.endswith("includes"):
				continue

			for file in files:
				if not file.endswith((".c", ".h")):
					continue

				curr_time = os.path.getmtime(root + "\\" + file)

				if file.endswith(".c"):
					c_files.append((file, root + "\\" + file))
				else:
					h_files.append((file, root + "\\" + file))
				
				curr_mod_times.append(f"{file}<=>{curr_time}")
	else:
		changed_files: list[str] = []

		with open(MOD_TIME_INFO_FILE, "r") as f:
			last_mod_times = f.read().splitlines()

		for (root,_,files) in os.walk('.\\source', topdown=True): 
			if root.endswith("includes"):
				continue

			for file in files:
				if not file.endswith((".c", ".h")):
					continue

				i = 0
				while i < len(last_mod_times):
					if last_mod_times[i].startswith(file):
						curr_time = os.path.getmtime(root + "\\" + file)
						prev_time = last_mod_times[i].split("<=>")[-1]

						if prev_time != str(curr_time):
							if file.endswith(".c"):
								c_files.append((file, root + "\\" + file))
							else:
								h_files.append((file, root + "\\" + file))
							changed_files.append(file)

						curr_mod_times.append(f"{file}<=>{curr_time}")
						last_mod_times.pop(i)
						i -= 1
						break
					i += 1
				
				if i == len(last_mod_times):
					curr_time = os.path.getmtime(root + "\\" + file)

					if file.endswith(".c"):
						c_files.append((file, root + "\\" + file))
					else:
						h_files.append((file, root + "\\" + file))
					changed_files.append(file)

					curr_mod_times.append(f"{file}<=>{curr_time}")
				
			# END FOR LOOP
		# END FOR LOOP

		changed = True
		depth = MAX_DEPTH
		while changed and depth > 0:
			changed = False
			depth -= 1
			for (root,_,files) in os.walk('.\\source', topdown=True): 
				if root.endswith("includes"):
					continue

				for file in files:
					if not file.endswith((".c", ".h")):
						continue
					
					if file in changed_files:
						continue

					found_chain = False
					with open(root + "\\" + file, "r") as f:
						for line in f:
							if line.find("include") != -1:
								for change_file in changed_files:
									if line.find(change_file) != -1:
										changed_files.append(file)
										changed = True
										found_chain = True
										break
								
								if found_chain:
									break
						# END FOR LOOP
					
					if found_chain:
						i = 0
						while i < len(last_mod_times):
							if last_mod_times[i].startswith(file):
								curr_time = os.path.getmtime(root + "\\" + file)
								prev_time = last_mod_times[i].split("<=>")[-1]

								if file.endswith(".c"):
									c_files.append((file, root + "\\" + file))
								else:
									h_files.append((file, root + "\\" + file))

								curr_mod_times.append(f"{file}<=>{curr_time}")
								last_mod_times.pop(i)
								i -= 1
								break
							i += 1
						
						if i == len(last_mod_times):
							curr_time = os.path.getmtime(root + "\\" + file)

							if file.endswith(".c"):
								c_files.append((file, root + "\\" + file))
							else:
								h_files.append((file, root + "\\" + file))

							curr_mod_times.append(f"{file}<=>{curr_time}")
				# END FOR LOOP
			# END FOR LOOP
		# END WHILE LOOP
	# END IF

	with open(BUILD_LOG_PATH, "w") as f:
		f.write("") # CLEAR FILE

	line = ""
	
	for (file_name, file_path) in h_files:
		line = f"{COMPILER_FLAGS} -c {file_path} -o {BUILD_OBJ_PATH}\\{file_name}.gch 2>> {BUILD_LOG_PATH}" 
		print(line)
		result = os.system(line)
		if result != 0:
			print(f"Error [{result}] on cmd:'{line}'")
			exit(1)

	for (file_name, file_path) in c_files:
		line = f"{COMPILER_FLAGS} -I {BUILD_OBJ_PATH} -c {file_path} -o {BUILD_OBJ_PATH}\\{file_name}.o 2>> {BUILD_LOG_PATH}"
		print(line)
		result = os.system(line)
		if result != 0:
			print(f"Error [{result}] on cmd:'{line}'")
			exit(1)

	result = os.system(f"make final_compile 2>> {BUILD_LOG_PATH}")
	if result != 0:
		print(f"Error [{result}] on cmd:'{line}'")
		exit(1)

	with open(MOD_TIME_INFO_FILE, "w") as f:
		f.write("\n".join(curr_mod_times))
		f.write("\n")

if __name__ == "__main__":
	main()