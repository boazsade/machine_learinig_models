# machine_learinig_models
This would enable to train on verisous machine learning models using json API that would also enable cleaning the data and testing models prediction accuracy
This would have the source code and other utilities for building and running back end applications for processing data to valid machine learning/deep learning models
Please note that as of now you can only build on CentOS 7 OS host. While the code it produces can run on any of the latest Linux variants, to compile this on other Linux than
CentOS 7 would require that you would manualy set the required Kerenl development headers and point the linker to the location of the required C runtime libraries that can be
located on different locations on different Linux distros. but this
would only work for CentOS 7

#To generate a new version - 
* run "make version" from the root directory of this repository. This would build all aplications and run all unit tests, and at the end would generate a new install file under installs directoy inside this repo.
* In order for this to succeed you need to have code with 3rd parties libs cloned as well new to this repo (next to each other with the same parent dirctory).
* The first is the repository to which the scrip would place the new version ready to install, and the second contains external code that need for the build
* whe generating a install file it would overwrite the last install file that was generated. Please note that these files are not commited to the repo.
* to build without creating new version "make apps_build" This would build applications only, but not run the unit tests and not create install file.
* to run unit tests only "make apps_uts" 
* note that "make version" would run both unit tests and would build the version"
# to generate dependencies for the installation
* run the script prepare_3rd_party_version.sh
* this would generate a new installation only in case you made changes to the dependenices. Note that this would generate a new install file under installs directoy for installation of software that needed for this to run succesfully.

# building
* As noted this would only work on CentOS 7.
* run any of the make command to make sure that you are able to build
* note that if your dependencies failed you may need to run instalation file 
* you can run "make version" from the root directory or make app in one of the sub directories under src/apps, or "make build_lib" inside one of the sub directories in src/libs or one of the unit test in one of the "ut" direcotries
* you can write and build tests by creating new directory under src/tests and after running "make test" run the new test under "build/tests/<release|debug>/<test name>"
* you can create debug version by adding "DEBUG=1" after the "make" command and before the make target that you are running.

