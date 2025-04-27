import subprocess
import sys
import re

def normalize_output(output):
    """Normalize output by replacing ft_ping with ping and masking times"""
    normalized = output.replace("ft_ping", "ping")
    # Mask times like "time=1.234 ms" with "time=XXX ms"
    normalized = re.sub(r"time=\d+\.\d+ ms", "time=XXX ms", normalized)
    return normalized

def compare_ping_times(output1, output2, tolerance_ms=30):
    """Compare ping times with tolerance"""
    times1 = [float(match) for match in re.findall(r"time=(\d+\.\d+) ms", output1)]
    times2 = [float(match) for match in re.findall(r"time=(\d+\.\d+) ms", output2)]
    
    if len(times1) != len(times2):
        return False
    
    # print difference between times
    for t1, t2 in zip(times1, times2):
        if abs(t1 - t2) > tolerance_ms:
            print(f"Time difference: {t1} ms vs {t2} ms")
    return all(abs(t1 - t2) <= tolerance_ms for t1, t2 in zip(times1, times2))

def run_test(title, arguments, printDiff=False):
    command1 = sys.argv[1] + " " + arguments
    command2 = sys.argv[2] + " " + arguments
    
    print(f"{title}")
    
    # Run commands
    result1 = subprocess.run(command1, shell=True, text=True,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    result2 = subprocess.run(command2, shell=True, text=True,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    
    output1 = result1.stdout.strip()
    output2 = result2.stdout.strip()
    status1 = result1.returncode
    status2 = result2.returncode
    
    # Compare
    time_match = compare_ping_times(output1, output2)
    norm_match = normalize_output(output1) == normalize_output(output2)
    status_match = (status1 == status2)

    # if last argument specified and verbose
    if len(sys.argv) > 4 and sys.argv[3] == "verbose":
        print("Output 1:\n", normalize_output(output1))
        print("Output 2:\n", normalize_output(output2))
    
    if norm_match and time_match and status_match:
        print("✅ PASS: Full match (output, times, and status)")
    elif not time_match:
        print("❌ FAIL: times differ")
    elif norm_match and status_match:
        print("❌ TIME DIFF: Output and status match but times differ >30ms")
    elif norm_match:
        print("⚠️ STATUS DIFF: Output matches but status differs")
    elif status_match:
        print("☑️ OUTPUT DIFF: Status matches but output differs")
    else:
        print("❌ FAIL: Output, times and status all differ")

    if printDiff:
        print("Output 1:\n", output1)
        print("Output 2:\n", output2)
        print("Status 1:", status1)
        print("Status 2:", status2)


# Example usage:
if __name__ == "__main__":

    print("Running comparison test...")
    print("✅ PASS: Outputs and status codes match!")
    print("☑️ PARTIAL PASS: Status codes match, but outputs differ!")
    print("❌ FAIL: status codes differ!")

    print("" + "=" * 50)
    if len(sys.argv) < 4 or not sys.argv[3] == "latency":
      run_test(
          "Test with invalid host <localhostt>",
          "localhostt",
      )

      run_test(
          "Test with invalid ip <1234.0.0.0>",
          "1234.0.0.0",
      )

      print("" + "=" * 50)
      run_test(
          "Test with invalid argument <localhost -cq>",
          "localhost -cq",
      )

      run_test(
          "Test with invalid argument <localhost -x>",
          "localhost -x")

      run_test(
          "Test with invalid argument <localhost -q-c>",
          "localhost -q-c")

      run_test(
          "Test with invalid argument <localhost -c--1>",
          "localhost -c--1")

      print("" + "=" * 50)

      run_test(
          "Test with valid options <localhost -c 1>",
          "localhost -c 1")
      
      run_test(
          "Test with valid options <localhost -c1>",
          "localhost -c1")
      
      run_test(
          "Test with valid options <localhost --c 1>",
          "localhost --c 1")
    
      run_test(
          "Test with broken ip <-c1 -W1 192.168.123.45>",
          "-c1 -W1 192.168.123.45")
    
      run_test(
          "Test with multiple hosts <localhost localhost localhost -c1>",
          "localhost localhost localhost -c1")

      run_test(
          "Test with multiple hosts <localhost oo google.com -c2>",
          "localhost oo google.com -c2")
    
    run_test(
        "Test latency<localhost -c10>",
        "localhost -c10 ")
  
    run_test(
        "Test latency <google.com -c10>",
        "google.com -c10")
    
    run_test(
        "Test latency <8.8.8.8 -c10>",
        "8.8.8.8 -c10")
    if len(sys.argv) == 5 and sys.argv[4] == 'deep':
        run_test(
        "Test latency with big packets <8.8.8.8 -c100>",
        "8.8.8.8 -c100")
        run_test(
        "Test latency with big packets <localhost -c100>",
        "localhost -c100"
    if len(sys.argv) < 4 or not sys.argv[3] == "latency":
      run_test(
          "Test value number range <localhost -W-1>",
          "localhost -W-1")
      
      run_test(
          "Test value number range <localhost -W-0>",
          "localhost -W-0")

    print("" + "=" * 50)

