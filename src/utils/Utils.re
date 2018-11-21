let str = ReasonReact.string;

module Group = {
  let byDate = input =>
    input->Belt.Array.reduce(
      Js.Dict.empty(),
      (dict, curr) => {
        let date = DateFns.format(curr##date, "dddd MMM DD");
        let setValue = Js.Dict.set(dict, date);
        let current = [|curr|];

        switch (Js.Dict.get(dict, date)) {
        | None => current->setValue
        | Some(old) => Belt.Array.concat(old, current)->setValue
        };

        dict;
      },
    );
};