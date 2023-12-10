# alpinetime-sw

source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr/
west build -b alpinetime_0_2
west flash
