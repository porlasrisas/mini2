# Pruebas interactivas simuladas para detectar leaks/FDs
set -e

# 1) echo simple y exit
printf 'echo hola\nexit\n' > /tmp/ms_input_1
# 2) comando inexistente
printf 'comandoxxx\nexit\n' > /tmp/ms_input_2
# 3) directorio como comando
printf '/\nexit\n' > /tmp/ms_input_3
# 4) permiso denegado (crear script sin x)
cat > /tmp/ms_noexec.sh <<'EOF'
#!/bin/sh
echo NO
EOF
chmod 600 /tmp/ms_noexec.sh
printf '/tmp/ms_noexec.sh\nexit\n' > /tmp/ms_input_4
# 5) ENOEXEC: archivo binario inválido (crear vacío y marcar x)
: > /tmp/ms_empty
chmod 700 /tmp/ms_empty
printf '/tmp/ms_empty\nexit\n' > /tmp/ms_input_5
# 6) PATH no encontrado (unset PATH y ejecutar foo)
printf 'unset PATH\nfoo\nexit\n' > /tmp/ms_input_6
# 7) redirecciones salida/append
printf 'echo a > /tmp/ms_f && echo b >> /tmp/ms_f && cat < /tmp/ms_f\nrm -f /tmp/ms_f\nexit\n' > /tmp/ms_input_7
# 8) fichero inexistente en input
printf 'cat < /tmp/no_such_file\nexit\n' > /tmp/ms_input_8
# 9) pipeline 2 cmds con SIGPIPE
printf 'yes | head -n1\nexit\n' > /tmp/ms_input_9
# 10) pipeline largo con redirs y touch en medio
printf 'ls | touch /tmp/ms_bar | echo fua > /tmp/ms_bar | cat /tmp/ms_bar\nrm -f /tmp/ms_bar\nexit\n' > /tmp/ms_input_10
# 11) builtin en pipeline y heredoc con expand/no-expand
printf 'export X=42\ncat <<EOF | grep 42\n$X\nEOF\ncat <<\'EOFn\'\n$X\nEOFn\nexit\n' > /tmp/ms_input_11
# 12) cd/error y env/export/unset
printf 'cd /nope\nexport A=1\nunset A\nenv | grep -E "^A=" || true\nexit\n' > /tmp/ms_input_12

