# Exercises for the LUMI 1-day trainings

This repository contains material for the LIUMI 1-day trainings. It will
be provided on the system in the training project for people participating
in the course.

The corresponding course notes and exercises will be kept in 
[the LUMI training materials web site](https://lumi-supercomputer.github.io/LUMI-training-materials/).

If you have an active project on LUMI, you may be able to make some or all of the 
exercises depending on the resources that have been allocated to you (as some need
CPU billing units while others need GPU billing units). to this end:

1.  Clone the repository. This process will create the subdirectory LUMI-1day-training
    in the directory where you execute the git command:
    ```
    git clone https://github.com/Lumi-supercomputer/LUMI-1day-training.git
    cd LUMI-1day-training
    ```

2.  There is a tag for each training. If you cannot find the tag in the notes of the training
    in the [the LUMI training materials web site](https://lumi-supercomputer.github.io/LUMI-training-materials/),
    you can just check the tags with:
    ```
    git tag
    ```
    Choose the tag that seems to correspond to the date the training was given.

3.  Checkout the tag to get the exercises as they were on that training day. E.g., for a training
    given on 28 March 2023, the tag would be 20230328 and you can use:
    ```
    git checkout 20230328
    ```
