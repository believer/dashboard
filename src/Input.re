let component = ReasonReact.statelessComponent("Input");

let make =
    (
      ~error: option(Belt.Result.t(Formality__Form.Validation.ok, string)),
      ~id,
      ~label,
      ~type_="text",
      ~value,
      ~onChange,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="mb2">
      <label className="db b mb2" htmlFor=id> label->Utils.str </label>
      <input
        className="b--black-10 db w-50 bw1 br2 ph3 pv2"
        type_
        id
        name=id
        onChange
        value
      />
      {
        switch (error) {
        | Some(Error(message)) =>
          <div className={Cn.make(["form-message", "failure"])}>
            message->Utils.str
          </div>
        | Some(Ok(Valid))
        | Some(Ok(NoValue))
        | None => ReasonReact.null
        }
      }
    </div>,
};
