import argparse
import csv
import matplotlib.pyplot as plt

from os import listdir, mkdir
from os.path import exists, isdir, isfile, join
from dataclasses import dataclass
from enum import Enum
from copy import deepcopy


class Operations(Enum):
    PUT = 'PUT'
    CONTAINS_KEY = 'LOOKUP'
    REMOVE = 'REMOVE'
    CONTAINS_KEY_FAILED = 'FAILED LOOKUP'


CONVERTER = {
    'put': Operations.PUT,
    'containsKey': Operations.CONTAINS_KEY,
    'remove': Operations.REMOVE,
    'containsKeyFailed': Operations.CONTAINS_KEY_FAILED
}


@dataclass
class HashMapResults:
    name: str
    ranges: list[int]
    results: dict[Operations, list[list[int]]]

    def get_average(self, operation: Operations) -> list[int]:
        return [sum(elements) / len(elements) for elements in zip(*self.results[operation])]

    def get_operations(self) -> list[Operations]:
        return [k for k, v in self.results.items()]


def get_results(files: list[str]) -> dict[str, list[HashMapResults]]:
    ranges = list()
    data = dict()

    operations = {
        Operations.PUT: [[] for _ in range(len(files))],
        Operations.CONTAINS_KEY: [[] for _ in range(len(files))],
        Operations.REMOVE: [[] for _ in range(len(files))],
        Operations.CONTAINS_KEY_FAILED: [[] for _ in range(len(files))]
    }

    for idx, filename in enumerate(files):
        with open(filename, 'r') as f:
            reader = csv.reader(f)
            next(reader, None)
            for row in reader:
                if row[1] not in ranges:
                    ranges.append(row[1])

                if row[2] not in data:
                    data[row[2]] = [HashMapResults(name, list(), deepcopy(operations)) for name in ('LL', 'DH', 'RH')]

                match row[0]:
                    case 'LL':
                        data[row[2]][0].results[CONVERTER[row[3]]][idx].append(int(row[4]))
                    case 'DH':
                        data[row[2]][1].results[CONVERTER[row[3]]][idx].append(int(row[4]))
                    case 'RH':
                        data[row[2]][2].results[CONVERTER[row[3]]][idx].append(int(row[4]))

    for _, v in data.items():
        for hm in v:
            hm.ranges = ranges

    return data


def format_number(number: int) -> str:
    if 1e3 <= number < 1e6:
        return f"{int(number / 1e3)}K"
    elif number >= 1e6:
        return f"{int(number / 1e6)}M"
    else:
        return str(int(number))


def main() -> None:
    parser = argparse.ArgumentParser(
        description='Arguments to configure a script')

    parser.add_argument(
        '--results',
        type=str,
        required=True,
        help='specify a path to result files')

    parser.add_argument(
        '--save',
        action='store_true',
        help='specify if generated plots should be saved')

    args = parser.parse_args()
    if not exists(args.results) and not isdir(args.results):
        raise FileNotFoundError('Cannot find specified files')

    files = [join(args.results, r) for r in listdir(args.results) if isfile(join(args.results, r))]
    results = get_results(files)

    mkdir('./assets') if args.save and not isdir('./assets') else None

    for lf, maps in results.items():
        ll, dh, rh = maps
        for operation in ll.get_operations():
            x = [format_number(int(n)) for n in ll.ranges]
            if operation == Operations.CONTAINS_KEY_FAILED:
                plt.plot(x, ll.get_average(operation), label=ll.name, linestyle='-', marker='o', markersize=3.5)
                plt.plot(x, dh.get_average(operation), label=dh.name, linestyle='-', marker='o', markersize=3.5)
                plt.plot(x, rh.get_average(operation), label=rh.name, linestyle='-', marker='o', markersize=3.5)
                plt.ylabel('Time [ns]')
            else:
                plt.plot(x, list(map(lambda ns: ns / 1000000, ll.get_average(operation))),
                         label=ll.name, linestyle='-', marker='o', markersize=3.5)
                plt.plot(x, list(map(lambda ns: ns / 1000000, dh.get_average(operation))),
                         label=dh.name, linestyle='-', marker='o', markersize=3.5)
                plt.plot(x, list(map(lambda ns: ns / 1000000, rh.get_average(operation))),
                         label=rh.name, linestyle='-', marker='o', markersize=3.5)
                plt.ylabel('Time [ms]')
            plt.xticks(x)
            plt.grid()
            plt.legend()
            plt.xlabel('Number of entries in hash map')
            plt.title(f'{operation.value} (load factor {lf})', fontweight='bold')
            plt.savefig(f'./assets/{"-".join(operation.value.split(" "))}-lf{lf}.png') if args.save else None
            plt.show()


if __name__ == '__main__':
    main()
