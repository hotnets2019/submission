from collections import defaultdict
import sys

NUM_BUCKETS = 50


class DeadlineAssigner:
    def __init__(self, buckets):
        self.buckets = buckets

    @staticmethod
    def load_from_file(filename):
        num_packets_to_fct = defaultdict(list)

        with open(filename) as deadlines_file:
            for line in deadlines_file:
                num_packets, flow_duration, *_ = line.split()
                num_packets_to_fct[float(num_packets)].append(float(flow_duration))

        num_packets_fcts = sorted(num_packets_to_fct.items())
        num_total_flows = sum(len(fcts) for _, fcts in num_packets_fcts)

        min_flows_in_a_bucket = num_total_flows // NUM_BUCKETS

        buckets = []
        current_num_packets = None
        current_bucket = []
        for num_packets, fcts in num_packets_fcts:
            current_num_packets = num_packets
            current_bucket.extend(fcts)
            if len(current_bucket) >= min_flows_in_a_bucket:
                buckets.append((current_num_packets, max(current_bucket)))
                current_num_packets = None
                current_bucket = []

        if current_num_packets is not None:
            buckets.append((current_num_packets, max(current_bucket)))

        return DeadlineAssigner(buckets)

    def get_deadline(self, num_packets):
        i_upper = 0
        while i_upper < len(self.buckets) - 1 and self.buckets[i_upper][0] < num_packets:
            i_upper += 1
        return self.buckets[i_upper][1]


def count_satisfied_deadlines(deadline_assigner: DeadlineAssigner, flow_stat_filename: str):
    total = 0
    satisfied = 0
    with open(flow_stat_filename) as f:
        for line in f:
            num_packets, fct, *_ = line.strip().split()
            num_packets = float(num_packets)
            fct = float(fct)

            total += 1
            if fct <= deadline_assigner.get_deadline(num_packets):
                satisfied += 1

    print(f'{satisfied}/{total}')


if __name__ == '__main__':
    ideal_deadlines_filename = sys.argv[1]
    flow_stat_filename = sys.argv[2]
    count_satisfied_deadlines(DeadlineAssigner.load_from_file(ideal_deadlines_filename), flow_stat_filename)






