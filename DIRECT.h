#ifndef DIRECT_H
#define DIRECT_H

void change(uint16_t(&power)[4], uint8_t(&data_upr)[6]) {
	for (uint8_t i = 0; i < 4; i++)
	{
		if (data_upr[1]>=127)
		{
			power[i] = map(data_upr[1], 127, 255, 800, 2300);
		}
	}
}

#endif // DIRECT_H