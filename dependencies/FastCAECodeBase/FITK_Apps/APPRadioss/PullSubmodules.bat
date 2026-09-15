git submodule foreach --recursive "branch=$(git symbolic-ref --short -q HEAD); if [ -n \"$branch\" ]; then echo Pulling $name on branch $branch; git pull origin \"$branch\"; else echo Skipping $name because HEAD is detached; fi"

git pull

pause
