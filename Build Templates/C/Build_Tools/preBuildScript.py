# from file import main as file_main

import time, types

sub_build_processes: list[tuple[str, types.FunctionType]] = [
	# ("name", file_main),
]

def main():
	dash_num = 60
	start_time = time.time()
	print("\nStarting pre-build process...")
	print("-" * dash_num + "\n")
	
	for name, func in sub_build_processes:
		print(f"\tBuilding {name} ...".ljust(dash_num - 23), end="")
		func()
		print(f"Finished")

	print("\n" + "-" * dash_num)
	print(f"Finished pre-build process in {time.time() - start_time:0.3f}s")


if __name__ == "__main__":
	main()
