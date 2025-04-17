import re
def extract_rtt(filename):
    return [float(re.search(r'time=(\d+\.\d+)', line).group(1)) for line in open(filename) if 'time=' in line]
standard_rtt = extract_rtt('standard_ping.txt')
my_rtt = extract_rtt('my_ping.txt')
for i, (std, mine) in enumerate(zip(standard_rtt, my_rtt), 0):
    diff = abs(std - mine)
    print(f"Packet {i}: Standard={std} ms, Mine={mine} ms, Diff={diff:.3f} ms, {'Pass' if diff <= 30 else 'Fail'}")
