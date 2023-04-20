# Modified_Linux_based-cp_mv_command
Modified copy and move command for linux


Write a C program dircmx that copies or moves an entire directory sub-tree rooted at a specific
path in the home directory to a specific destination folder (also in the home directory) along with
only those file types specified by the extension list.
Synopsis :
dircmx [source_dir] [destination_dir] [options] <extension list>
 Both source_dir and destination_dir can be either absolute or relative paths, but must
belong to the home directory hierarchy.
 If the destination_dir folder is not present in the home directory hierarchy, it must be
created
 Options
o -cp copy the directory rooted at source_dir to destination_dir and do not delete
the directory rooted at source_dir
o -mv move the directory rooted at source_dir to destination_dir and delete the
sub-tree rooted at source_dir
 Extension List: up to 6 file extensions can provided ( c , pdf, txt etc.)
o If the file extensions are listed with -cp, only those file types listed in the
extension must be copied (along with the tree rooted at source_dir to the
destination_dir. All the folders must be copied as per the hierarchy.
o If extensions are listed with -mv , only those file types must be moved (along with
the tree rooted at source_dir to the destination_dir. All the folders must be copied
as per the original hierarchy and the directory sub-tree rooted at source_dir must
be deleted.
o If the extension list is not provided, all files and folders must copied or moved
depending on -cp or -mv
Requirement:
You must use the system call nftw() that allows you to traverse a file tree. This system call will
recursively visit all the files/directories present in the tree and will call you own function (a
function that you pass as a parameter). You need to read the manual of nftw() before you start
working on your assignment.
Page 2 of 2
Sample Runs
 $ dircmx ./folder1 ./folder2/folder3 -cp txt pdf
o This will copy the entire directory tree rooted at ./folder1 to ./folder2/folder3 and
will only copy .txt and .pdf files into the respective folders in the hierarchy
 $ dircmx ~/folder1 ~/folder3 -mv
o This will move the entire directory tree rooted at ~/folder1 to ~/folder3 (along
with all the files and folders as per the source_dir hierarchy. The original directory
tree rooted at source_dir will be deleted.
Appropriate error messages must be displayed by the program based on the specifications.
