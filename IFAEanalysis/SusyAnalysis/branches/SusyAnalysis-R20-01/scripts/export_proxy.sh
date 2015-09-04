#!/bin/bash

uid=`id -u $USER`

export X509_USER_PROXY=/nfs/at3/scratch/$USER/x509up_u$uid
