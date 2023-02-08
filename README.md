# 21202-operating-systems-part1

## Структура репозитория

У каждого студента есть своя папка для кода лабораторных.
Внутри именной папки нужно располагать папки с кодом задачек, в качестве названия таких папок нужно использовать номер лабораторной, чтобы пути папок получались по шаблону *имя аккаунта*/*номер лабораторной*.
Не нужно располагать лабораторные в корневой директории (например, `21202-operating-systems-part1/lab1`), так как после прохождения code review будут коллизии в наименовании директорий (например, несколько папок lab1, несколько lab2).

### Алгоритм прохождения code review

1. если нет ветки со своим именем и номером задачки, создается ветка со своим именем и номером задачки от main ветки
2. в эту ветку в папку со своим именем, а в ней в папку с номером нужной лабораторной коммитится код задачки
3. создается pull request из ветки с кодом лабораторной в ветку main
4. преподаватель проводит сode review
5. если есть замечания, нужно поправить код лабораторной и повторить шаг 2 и 3, на шаге 3 нужно обновить существующий pull request
6. если замечаний нет, pull request принимается, code review считается пройденным

### Примечания

- коммитить нужно весь код необходимый для сборки программки из кода, собранный **бинарник** коммитить не нужно (в частности, в файле `.gitignore` представлены файлы, которые обычно не коммитят)
- также, если есть **тесты**, подтверждающие работоспособность конечного решения, рекомендуется их тоже закоммитить и приложить минимальную инструкцию по запуску теста
- в преамбуле **pull request**'а рекомендуется написать примерную схему работы вашего решения, чтобы ускорить процесс review cо стороны преподавателя и уменьшить количество недопониманий
- **main ветка** должна быть защищена от записи, коммитить туда не надо (все изменения туда проходят только черз pull request)
- при указании веток можно использовать разделитель `/`, тогда в визуальных Git (например, IntelliJ IDEA / CLion, SmartGit, Sourcetree, GitKraken) средах ветки будут отображаться вложенными. В этом случае рекомендуется не называть одинаково ветку и папку, например `r.izhitskii/lab1` из-за неоднозначности при `checkout`.

## Правила написания кода

1. код должен запускаться на выделенном сервере (`sblab03.nsu.ru:22022`)
2. код должен быть production-ready во всех смыслах
3. код должен компилироваться в режиме максимальной паники компилятора без ошибок (error) и предупреждений (warning). Если warining не удается исправить без нарушения остальных правил, стоит этот случай согласовать с преподавателем.
4. программа не должна падать ни при каких вариантах запуска и входных параметрах
5. все логически значимые ошибки должны быть обработаны, и внятные ошибки быть выданы в терминал в случае возникновения ошибки
6. все использованные ресурсы должны быть грамотно освобождены (graceful shutdown) при любом валидном варианте завершения программы, кроме принудительного `kill`; код должен бережно относится к выделяемым ресурсам
7. код не должен приводить к undefined behaviour
