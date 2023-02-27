#! /bin/bash

tar -vcf exercises.tar --exclude "solution*.sh" --exclude "*.x" --exclude ".ptp*" --exclude "slurm*.out" LICENSE CPE EasyBuild
