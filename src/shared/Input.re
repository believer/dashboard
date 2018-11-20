let component = ReasonReact.statelessComponent("Input");

let make =
    (
      ~error: option(Belt.Result.t(Formality.ok, string)),
      ~id,
      ~label,
      ~help=None,
      ~type_="text",
      ~value,
      ~onChange,
      _children,
    ) => {
  ...component,
  render: _self => {
    let hasError =
      switch (error) {
      | Some(Error(_)) => true
      | _ => false
      };

    <div className="mb3 w-50">
      <label className="db b mb2" htmlFor=id> label->Utils.str </label>
      <input
        className={
          Cn.make([
            "b--black-10 db w-100 ba bw1 br2 ph3 pv2",
            "b--red"->Cn.ifTrue(hasError),
          ])
        }
        type_
        id
        name=id
        onChange
        value
      />
      {
        switch (help) {
        | Some(text) =>
          <div className="f6 w-75 lh-copy mt1 mid-gray"> ...text </div>
        | None => ReasonReact.null
        }
      }
      {
        switch (error) {
        | Some(Error(message)) =>
          <div className="mt1 red w-75 f6"> message->Utils.str </div>
        | _ => ReasonReact.null
        }
      }
    </div>;
  },
};