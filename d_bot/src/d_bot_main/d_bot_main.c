

void follow_line()
{
	u32_t r, l;

	gpio_pin_read(port_a, IR_RIGTH, &r);
	gpio_pin_read(port_a, IR_LEFT, &l);
	if (l) {
		while (l) {
			gpio_pin_read(port_a, IR_LEFT, &l);
			drive(leftwards);
		}
	} else if (r) {
		while (r) {
			gpio_pin_read(port_a, IR_RIGTH, &r);
			drive(rightwards);
		}
	} else {
		drive(forwards);
	}
}