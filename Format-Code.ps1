Get-ChildItem -Path .\src\ -Include *.cpp, *-hpp -Recurse | ForEach-Object { & clang-format --verbose -i $_.FullName --style=file }
